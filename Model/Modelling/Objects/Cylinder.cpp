#include "Cylinder.hh"

Cylinder::Cylinder() {
    center = vec3(0.0,0.0,0.0);
    radius = 2.0;
    height = 4.0;
}


Cylinder::Cylinder(vec3 center, float radius, float height, float data) :Object(data) {
    this->center = center;
    this->radius = radius;
    this->height = height;
}


Cylinder::Cylinder(float data) :Object(data) {
    center = vec3(0,0,0);
    radius = 2.0f;
    height = 4.0f;
}



bool Cylinder::hit(Ray &raig, float tmin, float tmax, HitInfo& info) const {
    float a,b,c;
    float rx,rz;
    float jx, jz;
    rx=raig.getDirection().x;
    rz=raig.getDirection().z;
    float t[4]={HUGE_VALF, HUGE_VALF, HUGE_VALF, HUGE_VALF};

    /*
     * Para calcular la intersección de un rayo de luz con un cilindro, se pueden seguir los siguientes pasos:
     * Ecuacion del rayo:
     * P(t) = P_0 + t * D
     * x = x_0 + t * dx
     * y = y_0 + t * dy
     * z = z_0 + t * dz
     *
     * Ecuación del cilindro en coordenadas cartesianas:
     * (x - a)^2 + (y - b)^2 = r^2
     * Donde (a, b) es el centro del círculo base del cilindro y r es el radio del cilindro.
     *
     * Escribe la ecuación del cilindro en coordenadas cartesianas:
     * x^2 + y^2 = r^2, 0 <= z <= h
     * Donde (x,y,z) son las coordenadas cartesianas del cilindro, r es el radio del cilindro y h es la altura del cilindro.
     *
     * Ponemos la ecuaciones del rayo de luz en la ecuación del cilindro:
     * at^2 + bt + c = 0
     * Donde:
     * a = dx^2 + dy^2
     * b = 2*(P0xdx + P0ydy)
     * c = P0x^2 + P0y^2 - r^2
     *
     * Y se puede resolver usando la fórmula general para ecuaciones cuadráticas:
     * t = (-b +/- sqrt(b^2 - 4ac)) / (2*a)
     */

    //Colision con la superficie curva.
    jx=raig.getOrigin().x - center.x;
    jz=raig.getOrigin().z - center.z;
    a = rx*rx + rz*rz;
    b = 2*(jx*raig.getDirection().x + jz*raig.getDirection().z);
    c = pow(jx, 2) + pow(jz, 2) - pow(radius,2);
    float discriminante=b*b-4*a*c;
    //Si existe solución real significa que intersecta con el cilindro infinito.
    if(discriminante>=0){
        float aux;

        //Sacamos las dos soluciones reales y comprobamos que estén dentro de las alturas del cilindo
        t[0]= (-b + sqrtf(discriminante)) / (2 * a);
        aux=raig.pointAtParameter(t[0]).y;
        if( t[0] < tmin || t[0] > tmax || (center.y + height) < aux || aux  < center.y ){
            t[0]=HUGE_VALF;
        }
        t[1]= (-b - sqrtf(discriminante)) / (2 * a);
        aux=raig.pointAtParameter(t[1]).y;
        if( t[1] < tmin || t[1] > tmax || (center.y + height) < aux || aux < center.y){
            t[1]=HUGE_VALF;
        }

        //Solo en caso de que no interseccione dos veces con la superficie curva miramos las tapas
        if(t[0] == HUGE_VALF || t[1] == HUGE_VALF) {
            t[2] = (center.y - raig.getOrigin().y) / raig.getDirection().y;//tapa inferior
            t[3] = (center.y + height - raig.getOrigin().y) / raig.getDirection().y;//tapa superior

            vec3 p = raig.pointAtParameter(t[2]);
            if (t[2]< tmin || t[2] > tmax || (pow(p.x - center.x, 2) + pow(p.z - center.z, 2) - pow(radius, 2)) > 0) {
                t[2] = HUGE_VALF;
            }

            p = raig.pointAtParameter(t[3]);
            if (t[3]< tmin || t[3] > tmax || (pow(p.x - center.x, 2) + pow(p.z - center.z, 2) - pow(radius, 2)) > 0) {
                t[3] = HUGE_VALF;
            }
        }

        //Si alguna de las intersecciones no es válida miramos también las tapas del cilindro.

        //Cogemos el mínimo.
        float min = t[0];
        int index_min = 0;

        for( int i = 1; i < 4 ; i ++){
            if(t[i] < min){
                index_min=i;
                min=t[i];
            }
        }
        if(min==HUGE_VALF || min < tmin || min>tmax){
            return false;
        }
        info.t = min;
        info.p = raig.pointAtParameter(info.t);
        info.mat_ptr = material.get();

        //Si la intersección seleccionada es con la superficie curva...
        if(index_min<2){
            /* Calculamos la normal como si el punto de intersección estuviera a la altura de la misma
             * base del cilindro. */
            info.normal = (vec3(info.p.x,center.y,info.p.z)-center)/radius;
        }
        //Si la intersección es con la tapa inferior
        else if(index_min==2){
            info.normal = vec3(0,-1,0);
        }
        //Si la intersección es con la tapa superior
        else{
            info.normal = vec3(0,1,0);
        }
        return true;
    }

    return false;
}


void Cylinder::aplicaTG(shared_ptr<TG> t) {
    if (dynamic_pointer_cast<TranslateTG>(t)) {
        // Per ara només es fan les translacions
        vec4 c(center, 1.0);
        c = t->getTG() * c;
        center.x = c.x; center.y = c.y; center.z = c.z;
    }
    //TODO: Cal ampliar-lo per a acceptar Escalats

}


void Cylinder::read (const QJsonObject &json)
{
    Object::read(json);

    if (json.contains("radious") && json["radious"].isDouble()) {
        radius = json["radious"].toDouble();
    }

    if (json.contains("h") && json["h"].isDouble()) {
        height = json["h"].toDouble();
    }

    if (json.contains("center") && json["center"].isArray()) {
        QJsonArray auxVec = json["center"].toArray();
        center[0] = auxVec[0].toDouble();
        center[1] = auxVec[1].toDouble();
        center[2] = auxVec[2].toDouble();
    }
}


void Cylinder::write(QJsonObject &json) const
{
    Object::write(json);

    QJsonArray auxArray;
    json["radious"] = radius;
    json["h"] = height;

    auxArray.append(center[0]);auxArray.append(center[1]);auxArray.append(center[2]);
    json["center"] = auxArray;
}


void Cylinder::print(int indentation) const
{
    Object::print(indentation);

    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << indent << "center:\t" << center[0] << ", "<< center[1] << ", "<< center[2] << "\n";
    QTextStream(stdout) << indent << "radious:\t" << radius<< "\n";
    QTextStream(stdout) << indent << "h:\t" << height<< "\n";
}
