#include "node.h"
#include <iostream>
node::node(){
    std::vector<int> beliefs (0,16);
    std::vector<int> MsgTop(0,16);
    std::vector<int> MsgBottom(0,16);
    std::vector<int> MsgRight(0,16);
    std::vector<int> MsgLeft(0,16);
       
}
node::node(int grayScale){
    grayScaleValue = grayScale;
    std::vector<int> beliefs (0,16);
    std::vector<int> MsgTop(0,16);
    std::vector<int> MsgBottom(0,16);
    std::vector<int> MsgRight(0,16);
    std::vector<int> MsgLeft(0,16);
       
}

