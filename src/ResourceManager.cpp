// MaiBo
// Copyright(c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

#include <maibo/ResourceManager.h>

#include <maibo/ResourceFuture.h>
#include <maibo/Task.h>
#include <maibo/TaskManager.h>

using namespace maibo;
using namespace std;

#if defined(MAIBO_PLATFORM_DESKTOP) || defined(__EMSCRIPTEN__)

int ResourceManager::checkFileExists(const string& path)
{
    auto f = fopen(path.c_str(), "rb");
    if (!f)
        return 1;

    fclose(f);
    return 0;
}

#endif

#if defined(MAIBO_PLATFORM_DESKTOP)

namespace
{
    class GetFileTask : public Task
    {
    public:
        GetFileTask(const std::string& path)
            : m_path(path)
        {
            future = make_shared<ResourceFuture<int>>();
        }

        bool Execute() override
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

ResourceFuturePtr<int> ResourceManager::GetFileAsync(const string& path)
{
    auto task = new GetFileTask(path);
    TaskManager::instance().pushTask(task);
    return task->future;
}

#elif defined(__EMSCRIPTEN__)

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

ResourceFuturePtr<int> ResourceManager::GetFileAsync(const string& path)
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
