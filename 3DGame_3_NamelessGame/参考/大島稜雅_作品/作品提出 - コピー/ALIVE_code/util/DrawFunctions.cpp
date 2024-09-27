#include "DrawFunctions.h"
#include <DxLib.h>
#include <cassert>

namespace Graph {

    //グラフを読み込む関数
    int Graph::LoadGraph(const char* path)
    {
        int handle = DxLib::LoadGraph(path);
        assert(handle != -1);

        return handle;
    }

    //読み込んだグラフを引数の値によって分割描画する
    int Graph::DrawRectRotaGraph(const float x, const float y, const int left, const int top, const int width, const int height, const float scale, const float angle, const int handle, const  bool transFlag, const  bool tumFlag)
    {
        return DrawRectRotaGraphF(x, y, left, top, width, height, scale, angle, handle, transFlag, tumFlag);
    }

}

