#pragma once

#include "Object.hh"
#include "Model/Modelling/Animation.hh"
#include "Model/Modelling/TG/TranslateTG.hh"

class Triangle: public Object  {
public:
    Triangle();
    Triangle(vec3 vertex1, vec3 vertex2, vec3 vertex3, float data);
    Triangle(float data);

    virtual ~Triangle() {}

    virtual bool hit(Ray& r, float tmin, float tmax, HitInfo& info) const override;

    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;


    vec3  getVertex1() { return vertex1;};
    vec3  getVertex2() { return vertex2;};
    vec3  getVertex3() { return vertex3;};

private:
    // Vertice 1 del triangulo
    vec3 vertex1;
    // Vertice 2 del triangulo
    vec3 vertex2;
    // Vertice 3 del triangulo
    vec3 vertex3;
};
