#include<vector>
#ifndef NODE_H
#define NODE_H
class node {
public:    
    
    node();
    node(int grayScaleValue);  
    int getGrayScaleValue (){return grayScaleValue;}
    std::vector<int> getBelief(){return beliefs;}
    std::vector<int> getMsgTop(){return msgTop;}
    std::vector<int> getMsgBottom(){return msgBottom;}
    std::vector<int> getMsgRight(){return msgRight;}
    std::vector<int> getMsgLeft(){return msgLeft;}
    
private:
    int grayScaleValue;
    std::vector<int> beliefs;
    std::vector<int> msgTop;
    std::vector<int> msgBottom;
    std::vector<int> msgRight;
    std::vector<int> msgLeft;
};
#endif
