#include "node.h"
#include <iostream>

//Constructors
node::node() : beliefs (16,0), msgTop(16,0), msgBottom(16,0), msgRight(16,0), msgLeft(16,0){
}
node::node(int gsv) : beliefs (16,0), msgTop(16,0), msgBottom(16,0), msgRight(16,0), msgLeft(16,0){
    grayScaleValue = gsv;
}

//Getters
double node::getBelief(int k) {
    return beliefs.at(k);
}

double node::getMsgTop(int k) {
    return msgTop.at(k);
}

double node::getMsgBottom(int k) {
    return msgBottom.at(k);
}

double node::getMsgLeft(int k) {
    return msgLeft.at(k);
}

double node::getMsgRight(int k) {
    return msgRight.at(k);
}

//Setters
void node::setBelief(int k, double belief) {
    beliefs.at(k) = belief;
}

void node::setMsgTop(int k, double msg) {
    msgTop.at(k) = msg;
}

void node::setMsgBottom(int k, double msg) {
    msgBottom.at(k) = msg;
}

void node::setMsgLeft(int k, double msg) {
    msgLeft.at(k) = msg;
}

void node::setMsgRight(int k, double msg) {
    msgRight.at(k) = msg;
}