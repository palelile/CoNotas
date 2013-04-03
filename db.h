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

class db
{
public:
	db();
	QSqlDatabase * db_local;

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

//	CREATE TABLE "alumno_en_curso" (
//		"id" INTEGER PRIMARY KEY NOT NULL,
//	    "rut_alumno" INTEGER NOT NULL,
//	    "id_curso" INTEGER NOT NULL,
//	    "vigente" INTEGER NOT NULL,
//	    FOREIGN KEY("rut_alumno") REFERENCES personas(rut),
//	    FOREIGN KEY("id_curso") REFERENCES cursos(id_curso)
//	);
	bool alumo_en_curso(QString rut,
						QString curso);
	bool alumno_agregar(QString rut,
						QString curso);
	bool alumno_quitar(QString rut,
					   QString curso);
	QString profesor_curso(QString curso_id);
	void consolidar_temporal();

	/*CREATE TABLE "asignaturas" (
		"id_asignatura" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
		"id_curso" TEXT NOT NULL,
		"rut_profesor" INTEGER NOT NULL,
		"id_codigo" INTEGER NOT NULL,
		"ponderable" INTEGER NOT NULL,
		FOREIGN KEY("id_curso") REFERENCES cursos(id_curso),
		FOREIGN KEY("rut_profesor") REFERENCES personas(rut)
	);*/
	QString id_asignatura(QString id_curso, QString id_codigo);
	QString asignatura_desc(QString id_asignatura);
	QStringList asignaturas_curso(QString id_curso);
	bool agregar_asignatura(QString id_curso, QString rut_profesor, QString id_codigo, QString ponderable);

//	CREATE TABLE "pruebas" (
//	    "id_prueba" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
//	    "id_asignatura" INTEGER NOT NULL,
//	    "periodo" INTEGER NOT NULL,
//	    "coeficiente" INTEGER NOT NULL,
//	    "valida" INTEGER NOT NULL,
//	    "decripcion" TEXT NOT NULL,
//	    FOREIGN KEY("id_asignatura") REFERENCES asignaturas(id_asignatura)
//	);
	bool prueba_agregar(QString id_asignatura, QString periodo, QString coeficiente, QString valida, QString descripcion);
	bool prueba_agregar_a_curso(QString id_asignatura, QString id_prueba);
	QString prueba_id(QString id_asignatura, QString periodo, QString coeficiente, QString valida, QString descripcion);
	QStringList periodo_obtener(QString id_asignatura);
	QStringList periodo_de_curso(QString id_curso);

//	CREATE TABLE "notas" (
//	    "id_nota" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
//	    "id_prueba" INTEGER NOT NULL,
//	    "rut_alumno" INTEGER NOT NULL,
//	    "nota" REAL NOT NULL,
//	    FOREIGN KEY("rut_alumno") REFERENCES personas(rut),
//	    FOREIGN KEY("id_prueba") REFERENCES asignaturas(id_prueba)
//	);
	QStringList notas_asignatura_rut(QString id_asignatura, QString rut_alumno);
private:
	bool conectar_db();

};

#endif // DB_H
