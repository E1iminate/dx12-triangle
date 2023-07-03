/*************************************************************************
* Copyright 2023 Vladislav Riabov
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/

#pragma once
#include <Windows.h>

#include <string>

struct Window
{
public:
    Window(const std::wstring& name, size_t width, size_t height);
    void Run();
private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    std::wstring mName;
    std::wstring mClassName;
    size_t mWidth;
    size_t mHeight;
    HWND mHandle = NULL;
};
