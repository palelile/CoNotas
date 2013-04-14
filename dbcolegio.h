#ifndef DBCOLEGIO_H
#define DBCOLEGIO_H

#include "dblocal.h"

class dbColegio : public dbLocal
{
public:
    dbColegio();
    bool personas_agregar_muchas(QString datos);
    bool alumno_agregar(QString rut, QString curso);
    QString cursos_id(QString anio, QString tipo, QString nivel, QString letra);
    void consolidar_temporal();

    bool prueba_agregar(QString id_asignatura, QString periodo, QString coeficiente, QString valida, QString descripcion);
    bool prueba_agregar_a_curso(QString id_asignatura, QString id_prueba);
};

#endif // DBCOLEGIO_H
