#pragma once

#include "Object.hh"
#include "Model/Modelling/Animation.hh"
#include "Model/Modelling/TG/TranslateTG.hh"

class Cylinder: public Object  {
public:
    Cylinder();
    Cylinder(vec3 center, float radius, float height, float data);
    Cylinder(float data);

    virtual ~Cylinder() {}

    virtual bool hit(Ray& r, float tmin, float tmax, HitInfo& info) const override;
    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    vec3  getCenter() { return center;};
    float getRadius() { return radius;};
    float getHeight() { return height;}

private:
    // Centro del cilindro
    vec3 center;
    // Radio del cilindro
    float radius;
    // Altura del cilindro
    float height;
};
