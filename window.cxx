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

#include "window.hxx"
#include "common.hxx"

Window::Window(const std::wstring& name, size_t width, size_t height)
    : mName(name)
    , mClassName(name + L"Class")
    , mWidth(width)
    , mHeight(height)
{
  WNDCLASSW wc{};
  wc.lpfnWndProc = WindowProc;
  wc.lpszClassName = mClassName.c_str();

  RegisterClassW(&wc);
  mHandle = CreateWindowW(mClassName.c_str(), mName.c_str(), WS_BORDER | WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT, mWidth, mHeight, NULL, NULL, NULL, NULL);
  if (!mHandle)
    ErrorExit(__FUNCTIONW__);
}

void Window::Run()
{
  ShowWindow(mHandle, SW_NORMAL);

  MSG msg{};
  while (GetMessageW(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    }
    default:
        break;
  }
  return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}
