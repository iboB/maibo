// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

#include "ResourceManager.h"
#include "maibo/Assets/FileManager.h"
#include "maibo/common/ResourceTask.h"
#include "maibo/common/ResourceFuture.h"
#include "maibo/Task.h"
#include "maibo/TaskManager.h"
#include "maibo/Rendering/Shader.h"
#include "maibo/Rendering/GPUProgram.h"
#include "maibo/Rendering/Texture.h"

#include <fstream>

using namespace maibo;
using namespace std;


namespace
{
    class LoadShaderTask : public ResourceTask<ShaderPtr, vector<char>>
    {
    public:
        LoadShaderTask(const string& shaderName, const ShaderType::Type shaderType, ConstResourceFuturePtr<vector<char>> readFileFuture)
            : ResourceTask(readFileFuture)
            , m_shaderName(shaderName)
            , m_shaderType(shaderType)
        {
        }

        bool safeExecute() override
        {
            future->resource() = make_shared<Shader>(m_shaderType, m_shaderName);

            int error = !future->resource()->load(m_dependentFuture->resource());

            future->setErrorCode(error);
            future->setProgress(1.f);
            future->setDone();
            return true;
        }

    private:
        const string m_shaderName;
        const ShaderType::Type m_shaderType;
    };
}

ResourceFuturePtr<ShaderPtr> ResourceManager::loadShaderAsync(const std::string& path, ShaderType::Type type, bool alsoGetFile)
{
    auto future = FileManager::instance().readFileAsync(path, alsoGetFile);

    auto task = new LoadShaderTask(path, type, future);
    TaskManager::instance().pushTask(task);

    return task->future;
}

namespace
{
    class LoadGPUProgramTask : public ResourceTask<GPUProgramPtr, ShaderPtr>
    {
    public:
        LoadGPUProgramTask(const string& programName, ConstResourceFuturePtr<ShaderPtr> vsFuture, ConstResourceFuturePtr<ShaderPtr> fsFuture)
            : ResourceTask(fsFuture)
            , m_programName(programName)
            , m_vsFuture(vsFuture)
            , m_fsFuture(fsFuture)
        {
        }

        bool safeExecute() override
        {
            if (!m_vsFuture->isDone())
            {
                return false;
            }

            assert(m_fsFuture->isDone());

            future->resource() = make_shared<GPUProgram>(m_programName);

            future->resource()->attachShader(m_vsFuture->resource());
            future->resource()->attachShader(m_fsFuture->resource());

            int error = !future->resource()->link();

            future->setErrorCode(error);
            future->setProgress(1.f);
            future->setDone();
            return true;
        }

    private:
        const string m_programName;
        ConstResourceFuturePtr<ShaderPtr> m_vsFuture;
        ConstResourceFuturePtr<ShaderPtr> m_fsFuture;
    };
}

ResourceFuturePtr<GPUProgramPtr> ResourceManager::loadGPUProgramAsync(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool alsoGetFiles)
{
    auto vsFuture = loadShaderAsync(vertexShaderPath, ShaderType::Vertex, alsoGetFiles);
    auto fsFuture = loadShaderAsync(fragmentShaderPath, ShaderType::Fragment, alsoGetFiles);

    auto task = new LoadGPUProgramTask(vertexShaderPath + fragmentShaderPath, vsFuture, fsFuture);
    TaskManager::instance().pushTask(task);

    return task->future;
}

namespace
{
class LoadTextureTask : public ResourceTask<TexturePtr, int>
{
public:
    LoadTextureTask(const string& textureName, ConstResourceFuturePtr<int> getFileFuture)
        : ResourceTask(getFileFuture)
        , m_textureName(textureName)
    {
    }

    bool safeExecute() override
    {
        future->resource() = make_shared<Texture>(m_textureName);

        auto success = future->resource()->loadFromFile(m_textureName.c_str());

        future->setErrorCode(!success);
        future->setProgress(1.f);
        future->setDone();

        return true;
    }

private:
    const string m_textureName;
};
}

ResourceFuturePtr<TexturePtr> ResourceManager::loadTexture(const std::string& path, bool alsoGetFile)
{
    auto future = FileManager::instance().getFileAsync(path);

    auto task = new LoadTextureTask(path, future);
    TaskManager::instance().pushTask(task);

    return task->future;
}