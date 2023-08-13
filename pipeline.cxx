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

#include "common.hxx"
#include "pipeline.hxx"

Pipeline::Pipeline(const Window& window)
{

  ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(mDebugController.put())));
  mDebugController->EnableDebugLayer();

  winrt::com_ptr<IDXGIFactory1> factory;

  if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(factory.put()))))
    throw std::runtime_error("Failed to create a DXGIFactory!");

  if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(mDevice.put()))))
    throw std::runtime_error("Failed to create a D3D12Device!");

  D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
  commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
  commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
  commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  commandQueueDesc.NodeMask = 0u;

  ThrowIfFailed(mDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(mCommandQueue.put())));

  DXGI_SWAP_CHAIN_DESC swapChainDesc{};

  swapChainDesc.BufferCount = 2;
  swapChainDesc.BufferDesc.Width = window.GetWidth();
  swapChainDesc.BufferDesc.Height = window.GetHeight();
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = window.GetHandle();
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.Windowed = TRUE;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

  ThrowIfFailed(factory->CreateSwapChain(mCommandQueue.get(), &swapChainDesc, mSwapChain.put()));

  D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
  descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  descriptorHeapDesc.NumDescriptors = frameCount;
  descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
  descriptorHeapDesc.NodeMask = 0u;

  ThrowIfFailed(mDevice->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(mDescriptorHeap.put())));

  D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
  UINT rtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

  for (size_t frameIndex = 0u; frameIndex < descriptorHeapDesc.NumDescriptors; ++frameIndex)
  {
    ThrowIfFailed(mSwapChain->GetBuffer(frameIndex, IID_PPV_ARGS(mRenderTargets[frameIndex].put())));
    mDevice->CreateRenderTargetView(mRenderTargets[frameIndex].get(), nullptr, cpuDescriptorHandle);
    cpuDescriptorHandle.ptr += rtvDescriptorSize;
  }

  ThrowIfFailed(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.put())));
}

void Pipeline::InitializeAssets()
{
  winrt::com_ptr<ID3DBlob> signature;
  winrt::com_ptr<ID3DBlob> error;

  D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
  rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

  ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, signature.put(), error.put()));
  ThrowIfFailed(mDevice->CreateRootSignature(0u, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(mRootSignature.put())));

  ThrowIfFailed(mDevice->CreateCommandList1(0,
    D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
    D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE,
    IID_PPV_ARGS(mGraphicsCommandList.put())));

  ThrowIfFailed(mGraphicsCommandList->Reset(mCommandAllocator.get(), nullptr));

  FLOAT colorRGBA[4] = {1.f, 0.f, 0.f, 1.f};
  mGraphicsCommandList->ClearRenderTargetView(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), colorRGBA, 0, nullptr);
  mGraphicsCommandList->Close();

  ID3D12CommandList* lists[] = {mGraphicsCommandList.get()};
  mCommandQueue->ExecuteCommandLists(_countof(lists), lists);

  ThrowIfFailed(mSwapChain->Present(1, 0));
}
