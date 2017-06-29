#include "node.h"
#include <iostream>

int offset = 16;
//Constructors
node::node() : beliefs (offset,0), msgTop(offset,0), msgBottom(offset,0), msgRight(offset,0), msgLeft(offset,0){
}
//node::node(int gsv) : beliefs (16,0), msgTop(16,0), msgBottom(16,0), msgRight(16,0), msgLeft(16,0){
//    grayScaleValue = gsv;
//}

//Getters
float node::getBelief(int k) {
    return beliefs.at(k);
}

float node::getMsgTop(int k) {
    return msgTop.at(k);
}

float node::getMsgBottom(int k) {
    return msgBottom.at(k);
}

float node::getMsgLeft(int k) {
    return msgLeft.at(k);
}

float node::getMsgRight(int k) {
    return msgRight.at(k);
}

//Setters
void node::setBelief(int k, int belief) {
    beliefs.at(k) = belief;
}

void node::setMsgTop(int k, int msg) {
    msgTop.at(k) = msg;
}

void node::setMsgBottom(int k, int msg) {
    msgBottom.at(k) = msg;
}

void node::setMsgLeft(int k, int msg) {
    msgLeft.at(k) = msg;
}

void node::setMsgRight(int k, int msg) {
    msgRight.at(k) = msg;
}