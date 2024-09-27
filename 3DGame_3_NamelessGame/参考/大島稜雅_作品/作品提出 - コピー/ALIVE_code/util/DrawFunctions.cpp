#include "DrawFunctions.h"
#include <DxLib.h>
#include <cassert>

namespace Graph {

    //�O���t��ǂݍ��ފ֐�
    int Graph::LoadGraph(const char* path)
    {
        int handle = DxLib::LoadGraph(path);
        assert(handle != -1);

        return handle;
    }

    //�ǂݍ��񂾃O���t�������̒l�ɂ���ĕ����`�悷��
    int Graph::DrawRectRotaGraph(const float x, const float y, const int left, const int top, const int width, const int height, const float scale, const float angle, const int handle, const  bool transFlag, const  bool tumFlag)
    {
        return DrawRectRotaGraphF(x, y, left, top, width, height, scale, angle, handle, transFlag, tumFlag);
    }

}

