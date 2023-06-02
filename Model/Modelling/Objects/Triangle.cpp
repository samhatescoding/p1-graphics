#include "Triangle.hh"

Triangle::Triangle() {
    vertex1 = vec3(0.0,0.0,0.0);
    vertex2 = vec3(1.0,0.0,0.0);
    vertex3 = vec3(0.5,0.0,1.0);
}

Triangle::Triangle(vec3 ver1, vec3 ver2, vec3 ver3, float data) :Object(data) {
    vertex1 = ver1;
    vertex2 = ver2;
    vertex3 = ver3;
}

Triangle::Triangle(float data) :Object(data) {
    vertex1 = vec3(0,0,0);
    vertex2 = vec3(1,0,0);
    vertex3 = vec3(0.5,0,1);
}

bool Triangle::hit(Ray &raig, float tmin, float tmax, HitInfo& info) const {

    /*
     * Buscamos la ecuacion del plano que contiene al triangulo, para ello
     * utilizamos el producto cruz entre dos vectores que pertenecen al plano y
     * usamos uno de los vectores como punto de referencia.
     */

    vec3 v1 = vertex2 - vertex1;
    vec3 v2 = vertex3 - vertex1;
    vec3 normal = cross(v1, v2);
    normalize(normal);

    /*
     * Comprobamos si el rayo es ortogonal con la normal del plano
     * debido a que si son paralelos no hay intersecion.
     */

    if(dot(raig.getDirection(), normal) == 0){
        return false;
    }

    /*
     * Ecuación del plano: Ax * Nx + Ay * Ny + Az * Nz + d = 0,
     * donde (Nx, Ny, Nz) es la normal del plano.
     *
     * Calculamos D tomando el vertex1 usado como referencia
     */

    float d = -normal[0] * vertex1[0] - normal[1] * vertex1[1] - normal[2] * vertex1[2];

    /*
     * Encontrar el punto de intersección entre el rayo de luz y el plano que contiene al triángulo:
     * Para ello, se puede encontrar el valor del parámetro t que hace que el punto de la ecuación
     * del rayo de luz coincida con el punto de la ecuación del plano.
     *
     * Ecuacion del rayo:
     * X = Ox + T * Dx
     * Y = Oy + T * Dy
     * Z = Oz + T * Dz
     *
     * Sustituyendo la ecuación del rayo en la ecuación del plano y despejando t:
     * T = -Ax*Nx + Ay*Ny + Az*Nz + d / Dx*Nx + Dy*Ny + Dz*Zz
     */

    vec3 originRay = raig.getOrigin();
    vec3 vectorRay = raig.getDirection();
    float temp =  -normal[0]*originRay[0] - normal[1]*originRay[1] - normal[2]*originRay[2] - d;

    temp/= normal[0]*vectorRay[0] + normal[1]*vectorRay[1] + normal[2]*vectorRay[2];

    // Retornem false si no estem en el rang demanat
    if (temp > tmax || temp < tmin) {
        return false;
    }

    /*
     * Verificar si el punto de intersección está dentro del triángulo:
     * Una vez encontrado el punto de intersección, se debe verificar si este punto se encuentra
     * dentro del triángulo. Para hacer esto, se pueden calcular las coordenadas baricéntricas del
     * punto con respecto a los vértices del triángulo y verificar si todas las coordenadas están
     * entre 0 y 1. Si esto es cierto, entonces el punto se encuentra dentro del triángulo.
     */

    vec3 P = raig.pointAtParameter(temp);

    vec3 C;

    // aresta 0
    vec3 a0 = vertex2 - vertex1;
    vec3 vb0 = P - vertex1;
    C = cross(a0, vb0);
    if (dot(normal, C) < 0){
        return false;
    }

    // aresta 1
    vec3 a1 = vertex3 - vertex2;
    vec3 vb1 = P - vertex2;
    C = cross(a1, vb1);
    if (dot(normal, C) < 0){
        return false;
    }

    // aresta 2
    vec3 a2 = vertex1 - vertex3;
    vec3 vb2 = P - vertex3;
    C = cross(a2,vb2);
    if (dot(normal, C) < 0){
        return false;
    }


    info.t = temp;
    info.p = P;

    info.normal = normal;
    info.mat_ptr = material.get();
    return true;
}


void Triangle::aplicaTG(shared_ptr<TG> t) {
    if (dynamic_pointer_cast<TranslateTG>(t)) {
        //En un triangulo el escalado y la translacion se aplican sobre los 3 puntos,
        //no hace falta diferenciar que tipo de matriz TG nos estan pasando
        vec4 v1_bis(vertex1, 1.0);
        vec4 v2_bis(vertex2, 1.0);
        vec4 v3_bis(vertex3, 1.0);

        v1_bis = t->getTG() * v1_bis;
        v2_bis = t->getTG() * v2_bis;
        v3_bis = t->getTG() * v3_bis;

        vertex1.x = v1_bis.x; vertex1.y = v1_bis.y; vertex1.z = v1_bis.z;
        vertex2.x = v2_bis.x; vertex2.y = v2_bis.y; vertex2.z = v2_bis.z;
        vertex3.x = v3_bis.x; vertex3.y = v3_bis.y; vertex3.z = v3_bis.z;
    }
    //TODO: Cal ampliar-lo per a acceptar Escalats

}


void Triangle::read (const QJsonObject &json)
{
    Object::read(json);

    if (json.contains("p1") && json["p1"].isArray()) {
        QJsonArray auxVec = json["p1"].toArray();
        vertex1[0] = auxVec[0].toDouble();
        vertex1[1] = auxVec[1].toDouble();
        vertex1[2] = auxVec[2].toDouble();
    }

    if (json.contains("p2") && json["p2"].isArray()) {
        QJsonArray auxVec = json["p2"].toArray();
        vertex2[0] = auxVec[0].toDouble();
        vertex2[1] = auxVec[1].toDouble();
        vertex2[2] = auxVec[2].toDouble();
    }

    if (json.contains("p3") && json["p3"].isArray()) {
        QJsonArray auxVec = json["p3"].toArray();
        vertex3[0] = auxVec[0].toDouble();
        vertex3[1] = auxVec[1].toDouble();
        vertex3[2] = auxVec[2].toDouble();
    }
}


void Triangle::write(QJsonObject &json) const
{
    Object::write(json);

    QJsonArray auxArray1;
    auxArray1.append(vertex1[0]);auxArray1.append(vertex1[1]);auxArray1.append(vertex1[2]);
    json["p1"] = auxArray1;

    QJsonArray auxArray2;
    auxArray2.append(vertex2[0]);auxArray2.append(vertex2[1]);auxArray2.append(vertex2[2]);
    json["p2"] = auxArray2;

    QJsonArray auxArray3;
    auxArray3.append(vertex3[0]);auxArray3.append(vertex3[1]);auxArray3.append(vertex3[2]);
    json["p3"] = auxArray3;
}


void Triangle::print(int indentation) const
{
    Object::print(indentation);

    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << indent << "vertex1:\t" << vertex1[0] << ", "<< vertex1[1] << ", "<< vertex1[2] << "\n";
    QTextStream(stdout) << indent << "vertex2:\t" << vertex2[0] << ", "<< vertex2[1] << ", "<< vertex2[2] << "\n";
    QTextStream(stdout) << indent << "vertex3:\t" << vertex3[0] << ", "<< vertex3[1] << ", "<< vertex3[2] << "\n";
}
