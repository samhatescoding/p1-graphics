#include "Box.hh"

Box::Box() {
    vertexMin = vec3(-1.0,-1.0,-1.0);
    vertexMax = vec3(1.0,1.0,1.0);
    center = (vertexMin + vertexMax) / 2.f;
}

Box::Box(vec3 vMin, vec3 vMax, float data) :Object(data) {
    vertexMin = vMin;
    vertexMax = vMax;
    center = (vMin + vMax) / 2.f;
}

Box::Box(float data) :Object(data) {
    vertexMin = vec3(-1,-1,-1);
    vertexMax = vec3(1,1,1);
    center = (vertexMin + vertexMax) / 2.f;
}

bool Box::hit(Ray &raig, float tmin, float tmax, HitInfo& info) const {
    /*
     * Un cubo puede definirse mediante dos puntos correspondientes a las
     * coordenadas de dos esquinas del cubo enfrentadas diagonalmente:
     * vertexMin, vertexMax
     *
     * Para aplicar el algoritmo de Kay y Kajiya es mejor considerar
     * un cubo canónico centrado en el origen y radio la unidad,
     * vertex1 = (-1, -1, -1) y vertex2 = (1, 1, 1), y el rayo transformado
     * R’(t) = PO’ + D’ * t, con punto origen PO’ = (POx’, POy’, POz’)
     * y vector dirección unitario D’ = (dx’, dy’, dz’).
     */

    float EPSILON = 0.001;

    // Para el eje x y el eje y
    float t_x_Min = (vertexMin.x - raig.getOrigin().x) / raig.getDirection().x;
    float t_x_Max = (vertexMax.x - raig.getOrigin().x) / raig.getDirection().x;

    if(t_x_Min > t_x_Max){
        float aux = t_x_Max;
        t_x_Max = t_x_Min;
        t_x_Min = aux;
    }

    float t_y_Min = (vertexMin.y - raig.getOrigin().y) / raig.getDirection().y;
    float t_y_Max = (vertexMax.y - raig.getOrigin().y) / raig.getDirection().y;

    if(t_y_Min > t_y_Max){
        float aux = t_y_Max;
        t_y_Max = t_y_Min;
        t_y_Min = aux;
    }

    if ((t_x_Min > t_y_Max) || (t_y_Min > t_x_Max))
        return false;

    if (t_y_Min > t_x_Min)
        t_x_Min = t_y_Min;

    if (t_y_Max < t_x_Max)
        t_x_Max = t_y_Max;

    // Para el eje x y el eje z
    float t_z_Min = (vertexMin.z - raig.getOrigin().z) / raig.getDirection().z;
    float t_z_Max = (vertexMax.z - raig.getOrigin().z) / raig.getDirection().z;

    if (t_z_Min > t_z_Max){
        float aux = t_z_Max;
        t_z_Max = t_z_Min;
        t_z_Min = aux;
    }

    if ((t_x_Min > t_z_Max) || (t_z_Min > t_x_Max))
        return false;

    if (t_z_Min > t_x_Min)
        t_x_Min = t_z_Min;

    if (t_z_Max < t_x_Max)
        t_x_Max = t_z_Max;

    if(t_x_Min > tmin && t_x_Max < tmax){
        info.t = t_x_Min;
        info.p = raig.getOrigin() + (raig.getDirection() * t_x_Min);
        info.mat_ptr = material.get();


        /*La normal Ni’ en el punto de intersección es un vector unitario
         * perpendicular a la cara intersectada:
         *
         * Cara anterior: Ni’ = (0, 0, -1)
         * Cara posterior: Ni’ = (0, 0, 1)
         * Cara superior: Ni’ = (0, 1, 0)
         * Cara inferior: Ni’ = (0, -1, 0)
         * Cara izquierda: Ni’ = (-1, 0, 0)
         * Cara derecha: Ni’ = (1, 0, 0)
         *
         * De igual forma que en casos anteriores, la normal se corregirá para
         * devolverla al espacio global de coordenadas de la escena, y si el
         * rayo incide en alguna cara interior se tomará con el signo cambiado.
         */

        if(fabs(info.p.x - vertexMax.x) < EPSILON)
            info.normal = vec3(1,0,0);

        if(fabs(info.p.y - vertexMax.y) < EPSILON)
            info.normal = vec3(0,1,0);

        if(fabs(info.p.z - vertexMax.z) < EPSILON)
            info.normal = vec3(0,0,1);

        if(fabs(info.p.x - vertexMin.x) < EPSILON)
            info.normal = vec3(-1,0,0);

        if(fabs(info.p.y - vertexMin.y) < EPSILON)
            info.normal = vec3(0,-1,0);

        if(fabs(info.p.z - vertexMin.z) < EPSILON)
            info.normal = vec3(0,0,-1);

        return true;
    }
    return false;

}


void Box::aplicaTG(shared_ptr<TG> t) {
    if (dynamic_pointer_cast<TranslateTG>(t)) {
        // Per ara només es fan les translacions
        vec4 c(center, 1.0);
        c = t->getTG() * c;
        center.x = c.x; center.y = c.y; center.z = c.z;
    }
}

void Box::read (const QJsonObject &json)
{
    Object::read(json);

    if (json.contains("punt_min") && json["punt_min"].isArray()) {
        QJsonArray auxVec = json["punt_min"].toArray();
        vertexMin[0] = auxVec[0].toDouble();
        vertexMin[1] = auxVec[1].toDouble();
        vertexMin[2] = auxVec[2].toDouble();
    }

    if (json.contains("punt_max") && json["punt_max"].isArray()) {
        QJsonArray auxVec = json["punt_max"].toArray();
        vertexMax[0] = auxVec[0].toDouble();
        vertexMax[1] = auxVec[1].toDouble();
        vertexMax[2] = auxVec[2].toDouble();
    }
}


void Box::write(QJsonObject &json) const
{
    Object::write(json);

    QJsonArray auxArrayMin;
    auxArrayMin.append(vertexMin[0]);auxArrayMin.append(vertexMin[1]);auxArrayMin.append(vertexMin[2]);
    json["punt_min"] = auxArrayMin;

    QJsonArray auxArrayMax;
    auxArrayMax.append(vertexMax[0]);auxArrayMax.append(vertexMax[1]);auxArrayMax.append(vertexMax[2]);
    json["punto_max"] = auxArrayMax;

}


void Box::print(int indentation) const
{
    Object::print(indentation);

    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << indent << "punto_min:\t" << vertexMin[0] << ", "<< vertexMin[1] << ", "<< vertexMin[2] << "\n";
    QTextStream(stdout) << indent << "punto_max:\t" << vertexMax[0] << ", "<< vertexMax[1] << ", "<< vertexMax[2] << "\n";

}
