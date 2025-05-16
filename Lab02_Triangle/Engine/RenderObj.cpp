#include "stdafx.h"
#include "RenderObj.h"
#include "VertexBuffer.h"

RenderObj::RenderObj(const VertexBuffer* vertexBuffer) {
	mBuffer = vertexBuffer;
}

RenderObj::~RenderObj() {
	delete mBuffer;
}

void RenderObj::Draw() {
	mBuffer->Draw();
}