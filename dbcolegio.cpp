#include "dbcolegio.h"

dbColegio::dbColegio()
{
}

bool dbColegio::personas_agregar_muchas(QString datos)
{
    QSqlQuery consulta(*baseDatos);
    datos.truncate( datos.length() - 1 );
    consulta.prepare( "INSERT OR IGNORE INTO temporal "
                      "VALUES " + datos + ";");
    return consulta.exec();
}

bool dbColegio::alumno_agregar(QString rut,
                        QString curso)
{
    QSqlQuery consulta(*baseDatos);
    if ( buscar("id", "alumno_en_curso",
                "rut_alumno='" + rut + "' AND id_curso='" + curso + "'") )
        return false;
    consulta.prepare("INSERT INTO alumno_en_curso "
                     "(rut_alumno, id_curso, vigente) "
                     "VALUES (:rut, :curso, 1)");
    consulta.bindValue(":rut", rut);
    consulta.bindValue(":curso", curso);
    if (!consulta.exec())
        return false;
    consulta.clear();
    consulta.prepare("INSERT INTO notas (id_prueba,rut_alumno,nota) "
                     "SELECT id_prueba,'" + rut +"','0' FROM pruebas WHERE id_asignatura IN "
                     "(SELECT id_asignatura FROM asignaturas WHERE id_curso=:curso)");
    consulta.bindValue(":curso", curso);
    return consulta.exec();
}

QString dbColegio::cursos_id(QString anio, QString tipo, QString nivel, QString letra)
{
    return (anio + tipo + nivel + letra);
}

void dbColegio::consolidar_temporal()
{
    QSqlQuery consulta(*baseDatos);
    consulta.exec("INSERT OR IGNORE INTO 'personas' "
                  "SELECT run_alumno,nombre_alumno,ape_paterno_alumno,ape_materno_alumno,sal_genero,fecha_nacimiento "
                  "FROM temporal ORDER BY run_alumno");
    consulta.exec("INSERT OR IGNORE INTO 'cursos' "
                  "(id_curso,anio,tipo,nivel,letra,descripcion) "
                  "SELECT DISTINCT anio_trabajo||codigo_tipo_ensenanza||codigo_grado||letra_curso AS id_curso,anio_trabajo,codigo_tipo_ensenanza,codigo_grado,letra_curso,glosa_grado||' '||letra_curso AS letra "
                  "FROM temporal "
                  "ORDER BY codigo_tipo_ensenanza,codigo_grado,letra_curso");
    consulta.exec("INSERT OR IGNORE INTO 'alumno_en_curso' (rut_alumno,id_curso) "
                  "SELECT run_alumno AS rut_alumno,anio_trabajo||codigo_tipo_ensenanza||codigo_grado||letra_curso AS id_curso "
                  "FROM temporal "
                  "ORDER BY anio_trabajo,codigo_tipo_ensenanza,codigo_grado,letra_curso");
}

bool dbColegio::prueba_agregar(QString id_asignatura,
                        QString periodo,
                        QString coeficiente,
                        QString valida,
                        QString descripcion)
{
    QSqlQuery consulta(*baseDatos);
    consulta.prepare("INSERT INTO pruebas "
                     "(id_asignatura,periodo,coeficiente,valida,descripcion) "
                     "VALUES "
                     "(:id_asignatura,:periodo,:coeficiente,:valida,:descripcion)");
    consulta.bindValue(":id_asignatura", id_asignatura);
    consulta.bindValue(":periodo", periodo);
    consulta.bindValue(":coeficiente", coeficiente);
    consulta.bindValue(":valida", valida);
    consulta.bindValue(":descripcion", descripcion);
    if ( !consulta.exec())
        return false;
    QString id_prueba = buscar("id_prueba",
           "pruebas",
           "id_asignatura='" +id_asignatura+
           "' AND periodo='" +periodo+
           "' AND coeficiente='" +coeficiente+
           "' AND valida='" +valida+
           "' AND descripcion='"+descripcion+
           "'")->at(0);
    return prueba_agregar_a_curso(id_asignatura, id_prueba);
}

bool dbColegio::prueba_agregar_a_curso(QString id_asignatura, QString id_prueba)
{
    QSqlQuery consulta(*baseDatos);
    consulta.prepare("SELECT alumno_en_curso.rut_alumno "
                     "FROM asignaturas,alumno_en_curso "
                     "WHERE asignaturas.id_asignatura=:id_asignatura "
                     "AND alumno_en_curso.id_curso=asignaturas.id_curso;");
    consulta.bindValue(":id_asignatura", id_asignatura);
    if ( !consulta.exec() )
        return false;
    QString datos = "INSERT INTO notas (id_prueba,rut_alumno,nota) VALUES ";
    while ( consulta.next() )
        datos.append( "(" + id_prueba + "," + consulta.record().value(0).toString() + ",0)," );
    datos.truncate(datos.length()-1);
    datos.append(";");
    return consulta.exec(datos);
}
