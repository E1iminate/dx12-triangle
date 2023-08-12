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

#include <d3d12.h>
#include <dxgi1_2.h>
#include <winrt/base.h>

class Pipeline
{
public:
  Pipeline(const Window& window);
  void InitializeAssets();

private:
  winrt::com_ptr<ID3D12Debug> mDebugController;
  winrt::com_ptr<ID3D12Device10> mDevice;
  winrt::com_ptr<ID3D12CommandQueue> mCommandQueue;
  winrt::com_ptr<IDXGISwapChain> mSwapChain;
  winrt::com_ptr<ID3D12DescriptorHeap> mDescriptorHeap;
  winrt::com_ptr<ID3D12Resource> mRenderTargetView;
  winrt::com_ptr<ID3D12CommandAllocator> mCommandAllocator;
  winrt::com_ptr<ID3D12RootSignature> mRootSignature;
  winrt::com_ptr<ID3D12PipelineState> mGraphicsPipelineState;
  winrt::com_ptr<ID3D12GraphicsCommandList7> mGraphicsCommandList;

  static constexpr size_t frameCount = 2;
  winrt::com_ptr<ID3D12Resource> mRenderTargets[frameCount];
};
