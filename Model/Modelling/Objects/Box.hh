/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#pragma once

#include "Object.hh"
#include "Model/Modelling/Animation.hh"
#include "Model/Modelling/TG/TranslateTG.hh"
#include <iostream>

class Box: public Object  {
public:
    Box();
    Box(vec3 vMin, vec3 vMax, float data);
    Box(float data);

    virtual ~Box() {}

    virtual bool hit(Ray& r, float tmin, float tmax, HitInfo& info) const override;

    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;


    vec3 getVertexMin() { return vertexMin;};
    vec3 getVertexMax() { return vertexMax;}
    vec3 getCenter() { return center;}

private:
    // vertice extremo min
    vec3 vertexMin;
    // vertice extremo max
    vec3 vertexMax;
    // centro del cubo
    vec3 center;
};
