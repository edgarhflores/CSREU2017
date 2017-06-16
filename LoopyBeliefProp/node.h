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
    double getBelief(int k);
    double getMsgTop(int k);
    double getMsgBottom(int k);
    double getMsgLeft(int k);
    double getMsgRight(int k);
    //Setters
    void setBelief(int k, double belief);
    void setMsgTop(int k, double msg);
    void setMsgBottom(int k, double msg);
    void setMsgLeft(int k, double msg);
    void setMsgRight(int k, double msg);    
private:
    int grayScaleValue;
    std::vector<double> beliefs;
    std::vector<double> msgTop;
    std::vector<double> msgBottom;
    std::vector<double> msgRight;
    std::vector<double> msgLeft;
};
#endif
