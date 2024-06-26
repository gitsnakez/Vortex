// Copyright (c) 2023 snakEZ
// All rights reserved

#include "DeviceContext.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "ErrorDispatcher.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system):m_device_context(device_context), m_system(system)
{
}

DeviceContext::~DeviceContext()
{
	m_device_context->Release();
}

void DeviceContext::ClearRenderTargetColor(const SwapChainPtr& swap_chain, float Red, float Green, float Blue, float Alpha)
{
	FLOAT clear_color[] = {Red,Green,Blue,Alpha};
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv,clear_color);
	m_device_context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, swap_chain->m_dsv);
}

void DeviceContext::SetVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::SetIndexBuffer(const IndexBufferPtr& index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::DrawTriangleList(UINT vertex_count, UINT start_vertex_index)		
{
	drawcalls_iterator++;
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::DrawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	drawcalls_iterator++;
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	drawcalls_iterator++;
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::SetViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_device_context->RSSetViewports(1, &vp);
}

void DeviceContext::SetVertexShader(const VertexShaderPtr& vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::SetPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}

void DeviceContext::SetTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, UINT num_textures)
{
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];

	for (UINT i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->m_shader_res_view;
		list_sampler[i] = texture[i]->m_sampler_state;
	}

	m_device_context->VSSetShaderResources(0, 1, list_res);
	m_device_context->VSSetSamplers(0, num_textures, list_sampler);
}

void DeviceContext::SetTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, UINT num_textures)
{
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];

	for (UINT i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->m_shader_res_view;
		list_sampler[i] = texture[i]->m_sampler_state;
	}

	m_device_context->PSSetShaderResources(0, 1, list_res);
	m_device_context->PSSetSamplers(0, num_textures, list_sampler);
}

void DeviceContext::SetConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::SetConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}