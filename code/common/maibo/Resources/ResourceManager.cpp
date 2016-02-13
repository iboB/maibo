// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

#include "ResourceManager.h"
#include "ResourceTask.h"
#include "ResourceFuture.h"
#include "maibo/Task.h"
#include "maibo/TaskManager.h"
#include "maibo/Rendering/Shader.h"
#include "maibo/Rendering/GPUProgram.h"

#include <fstream>

using namespace maibo;
using namespace std;

int ResourceManager::checkFileExists(const string& path)
{
    auto io = SDL_RWFromFile(path.c_str(), "rb");
    if (!io)
    {
        return 1;
    }

    SDL_RWclose(io);
    return 0;
}

#if !defined(__EMSCRIPTEN__)

namespace
{
    class GetFileTask : public Task
    {
    public:
        GetFileTask(const std::string& path)
            : future(new ResourceFuture<int>)
            , m_path(path)
        {
        }

        bool execute() override
        {
            int result = ResourceManager::instance().checkFileExists(m_path);
            future->resource() = result;
            future->setErrorCode(result);
            future->setProgress(1.f);
            future->setDone();

            return true;
        }

        ResourceFuturePtr<int> future;

    private:
        std::string m_path;
    };
}

ResourceFuturePtr<int> ResourceManager::getFileAsync(const string& path)
{
    auto task = new GetFileTask(path);
    TaskManager::instance().pushTask(task);
    return task->future;
}

#else

namespace
{
    // Ensure a path exists of the browser FS. Creates it if needed
    // the Emscripten wget functions cannot create a path for a file they've gotten and there is no
    // C++ API that allows this.
    // Feature requested here: https://github.com/kripken/emscripten/issues/3524
    void EnsurePathExists(string path)
    {
        auto pos = path.rfind('/');
        if (pos != string::npos)
        {
            path.erase(pos);

            if(path.empty())
                return;

            EnsurePathExists(path); // Recurse for parent directories

            static string format =
                "try {\n"
                "var path = '%s';\n"
                "FS.mkdir(path);\n"
                "} catch(e) {}"; // Ignore exception that it exists
            size_t len = format.length() + path.length();

            char* js = new char[len];
            sprintf(js, format.c_str(), path.c_str());

            emscripten_run_script(js);

            delete[] js;
        }
    }
}

// emscripten_async_wget2 which allows us to have user data, associated with a wget
// Unfortunately it has a known bug (More info here: https://github.com/kripken/emscripten/issues/3537)
// ...hence these horrible global map
// Once the bug is fixed, this code will be rewritten to make use of the user data

unordered_map<string, ResourceFuturePtr<int>> GetFile_Futures;

namespace
{
    void OnGetResource(const char* path)
    {
        auto i = GetFile_Futures.find(path);

        // Asynchronously requested a resource without adding it to the global map
        assert(i != GetFile_Futures.end());

        ResourceFuturePtr<int> future = i->second;

        future->resource() = 0;
        future->setErrorCode(0);
        future->setProgress(1.f);
        future->setDone();

        GetFile_Futures.erase(i);
    }

    void OnErrorGettingResource(const char* path)
    {
        auto i = GetFile_Futures.find(path);

        // Asynchronously requested a resource without adding it to the global map
        assert(i != GetFile_Futures.end());

        ResourceFuturePtr<int> future = i->second;

        future->resource() = 1;
        future->setErrorCode(1);
        future->setProgress(1.f);
        future->setDone();

        GetFile_Futures.erase(i);
    }
}

ResourceFuturePtr<int> ResourceManager::getFileAsync(const string& path)
{
    auto future = make_shared<ResourceFuture<int>>();

    if(checkFileExists(path) == 0)
    {
        // file is here (it's either preloaded or already wgotten)
        // no need to get it
        future->resource() = 0;
        future->setErrorCode(0);
        future->setProgress(1.f);
        future->setDone();

        // just return a complete future
        return future;
    }

    EnsurePathExists(path);

    GetFile_Futures.emplace(std::piecewise_construct,
        forward_as_tuple(path), forward_as_tuple(future));

    emscripten_async_wget(path.c_str(), path.c_str(), OnGetResource, OnErrorGettingResource);

    return future;
}

#endif

namespace
{
    int InternalReadFile(const char* path, vector<char>& outData)
    {
        auto io = SDL_RWFromFile(path, "rb");

        if (!io)
        {
            return 1;
        }

        auto fileSize = SDL_RWseek(io, 0, RW_SEEK_END);

        SDL_RWseek(io, 0, RW_SEEK_SET);

        outData.resize(fileSize);

        SDL_RWread(io, outData.data(), 1, fileSize);

        return 0;
    }
}

std::vector<char> ResourceManager::readFile(const string& path)
{
    vector<char> ret;
    InternalReadFile(path.c_str(), ret);
    return ret;
}

namespace
{
    class ReadFileTask : public ResourceTask<vector<char>, int>
    {
    public:
        ReadFileTask(const std::string& path, ConstResourceFuturePtr<int> getFileFuture = nullptr)
            : ResourceTask(getFileFuture)
            , m_path(path)
        {
        }

        bool safeExecute() override
        {
            int error = InternalReadFile(m_path.c_str(), future->resource());
            future->setErrorCode(error);
            future->setProgress(1.f);
            future->setDone();
            return true;
        }

    private:
        string m_path;
    };
}

ResourceFuturePtr<std::vector<char>> ResourceManager::readFileAsync(const string& path, bool alsoGetFile)
{
    ReadFileTask* task;

    if (alsoGetFile)
    {
        auto future = getFileAsync(path);
        task = new ReadFileTask(path, future);
    }
    else
    {
        task = new ReadFileTask(path);
    }

    TaskManager::instance().pushTask(task);
    return task->future;
}

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
    auto future = readFileAsync(path, alsoGetFile);

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
            assert(m_vsFuture->isDone());
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