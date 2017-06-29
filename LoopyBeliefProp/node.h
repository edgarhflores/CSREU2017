#include<vector>
#ifndef NODE_H
#define NODE_H
class node {
public: 
    //Constructors
    node();
    //node(int grayScaleValue); 
    //Getters
    int getGrayScaleValue (){return grayScaleValue;}  
    float getBelief(int k);
    float getMsgTop(int k);
    float getMsgBottom(int k);
    float getMsgLeft(int k);
    float getMsgRight(int k);
    //Setters
    void setBelief(int k, int belief);
    void setMsgTop(int k, int msg);
    void setMsgBottom(int k, int msg);
    void setMsgLeft(int k, int msg);
    void setMsgRight(int k, int msg);    
private:
    int grayScaleValue;
    std::vector<int> beliefs;
    std::vector<int> msgTop;
    std::vector<int> msgBottom;
    std::vector<int> msgRight;
    std::vector<int> msgLeft;
};
#endif
