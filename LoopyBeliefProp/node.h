#include<vector>
#ifndef NODE_H
#define NODE_H
class node {
public: 
    //Constructors
    node();
    node(int grayScaleValue); 
    //Getters
    int getGrayScaleValue (){return grayScaleValue;}  
    float getBelief(int k);
    float getMsgTop(int k);
    float getMsgBottom(int k);
    float getMsgLeft(int k);
    float getMsgRight(int k);
    //Setters
    void setBelief(int k, float belief);
    void setMsgTop(int k, float msg);
    void setMsgBottom(int k, float msg);
    void setMsgLeft(int k, float msg);
    void setMsgRight(int k, float msg);    
private:
    int grayScaleValue;
    std::vector<float> beliefs;
    std::vector<float> msgTop;
    std::vector<float> msgBottom;
    std::vector<float> msgRight;
    std::vector<float> msgLeft;
};
#endif
