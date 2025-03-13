#include "Serializable.hh"

/* Default implementation does nothing; derived classes should override */
void Serializable::read(const QJsonObject &json) {}
void Serializable::write(QJsonObject &json) const {}
void Serializable::print(int indentation) const {}