//--------------------------------------------------------------------------------------
// Skybox.h
//
// A sky box rendering helper. Takes DDS cubemap as input. 
// 
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once

#include <GeometricPrimitive.h>
#include "SkyboxEffect.h"

namespace ATG
{
    class Skybox
    {
    public:
        Skybox(
            ID3D12Device* device,
            D3D12_GPU_DESCRIPTOR_HANDLE cubeTexture,
            const DirectX::RenderTargetState& rtState,
            const DirectX::CommonStates& commonStates)
        {
            using namespace DirectX;
            using namespace DirectX::SimpleMath;

            // Skybox effect
            EffectPipelineStateDescription skyPSD(&ATG::SkyboxEffect::VertexType::InputLayout,
                CommonStates::Opaque,
                CommonStates::DepthRead,
                CommonStates::CullClockwise,
                rtState,
                D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

            m_effect = std::make_unique<ATG::SkyboxEffect>(device, skyPSD);
            m_effect->SetTexture(cubeTexture, commonStates.LinearWrap());

            // Create cube with corners at ([-1, 1], [-1, 1], [-1, 1]) 
            m_cube = DirectX::GeometricPrimitive::CreateGeoSphere(2.f);
        }

        void XM_CALLCONV Update(DirectX::FXMMATRIX view, DirectX::CXMMATRIX projection)
        {
            m_effect->SetMatrices(view, projection);
        }

        void Render(ID3D12GraphicsCommandList* cmdList)
        {
            m_effect->Apply(cmdList);
            m_cube->Draw(cmdList);
        }

    private:
        std::unique_ptr<DirectX::GeometricPrimitive>    m_cube;
        std::unique_ptr<ATG::SkyboxEffect>              m_effect;
    };
}