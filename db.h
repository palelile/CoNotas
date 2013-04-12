#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QStringList>
#include <QString>
#include <QDate>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>

class db
{
public:
	db();

    // Funciones a ser portadas a distintos motores de bases de datos
    bool conectar_db();
    bool desconectar();
    int estado();

    // Función generica de agregado de datos
    bool agregar(QString tabla, QString campos, QString valores);

    // Modelo asociado a base de datos
    QSqlRelationalTableModel * modelo(QObject *parent);
    QSqlQueryModel * modeloConsulta(QObject *parent, QString consulta);



	/***************************************************************************
	 *	Tabla Personas
	 **************************************************************************/

	bool persona_agregar(QString rut, QString nombre,
						 QString apellido_paterno, QString apellido_materno,
						 QString sexo,QDate fecha_nacimiento);
	bool persona_modificar(QString rut, QString nombre,
						   QString apellido_paterno, QString apellido_materno,
						   QString sexo, QDate fecha_nacimiento);
	bool persona_buscar(QString rut);
	bool personas_agregar_muchas(QString datos);
	QString persona_nombrar(QString rut);
	QStringList persona_datos(QString rut);

	/***************************************************************************
	 *	Tabla Cursos
	 **************************************************************************/

	QStringList cursos_anios();
	QStringList cursos_tipos(QString anio);
	QStringList cursos_nivel(QString anio, QString tipo);
	QStringList cursos_letras(QString anio, QString tipo, QString nivel);
	QString cursos_id(QString anio, QString tipo, QString nivel, QString letra);
	bool cursos_agregar(QString anio, QString tipo, QString nivel, QString letra,
						QString rut_profesor, QString plan, QString descripcion);
	bool cursos_agregar_batch(QString datos);
	QStringList cursos_datos(QString id);
	bool alumo_en_curso(QString rut,
						QString curso);
	bool alumno_agregar(QString rut,
						QString curso);
	bool alumno_quitar(QString rut,
					   QString curso);
	QString profesor_curso(QString curso_id);
	void consolidar_temporal();
	QString id_asignatura(QString id_curso, QString id_codigo);
	QString asignatura_desc(QString id_asignatura);
	QStringList asignaturas_curso(QString id_curso);
	bool agregar_asignatura(QString id_curso, QString rut_profesor, QString id_codigo, QString ponderable);
	bool prueba_agregar(QString id_asignatura, QString periodo, QString coeficiente, QString valida, QString descripcion);
	bool prueba_agregar_a_curso(QString id_asignatura, QString id_prueba);
	QString prueba_id(QString id_asignatura, QString periodo, QString coeficiente, QString valida, QString descripcion);
	QStringList periodo_obtener(QString id_asignatura);
	QStringList periodo_de_curso(QString id_curso);
	QStringList notas_asignatura_rut(QString id_asignatura, QString rut_alumno);

private:
    QSqlDatabase * db_local;
};

#endif // DB_H
