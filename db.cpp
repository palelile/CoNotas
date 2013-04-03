#include "db.h"

db::db()
{
	conectar_db();
}

bool db::conectar_db()
{
	db_local = new QSqlDatabase();
	*db_local = QSqlDatabase::addDatabase("QSQLITE", "colegio.db");
	db_local->setDatabaseName("colegio.db");
	return db_local->open();
}

bool db::persona_agregar(QString rut,
						 QString nombre,
						 QString apellido_paterno,
						 QString apellido_materno,
						 QString sexo,
						 QDate fecha_nacimiento)
{
	QSqlQuery consulta(*db_local);
	consulta.prepare("INSERT INTO personas "
					 "(rut,nombre,apellido_paterno,apellido_materno,sexo,fecha_nacimiento) "
					 "VALUES (:rut,:nombre,:apellido_paterno,:apellido_materno,:sexo,:fecha_nacimiento)");
	consulta.bindValue(":rut", rut);
	consulta.bindValue(":nombre", nombre);
	consulta.bindValue(":apellido_paterno", apellido_paterno);
	consulta.bindValue(":apellido_materno", apellido_materno);
	consulta.bindValue(":sexo", sexo);
	consulta.bindValue(":fecha_nacimiento", fecha_nacimiento);
	return consulta.exec();
}

bool db::persona_modificar(QString rut,
						   QString nombre,
						   QString apellido_paterno,
						   QString apellido_materno,
						   QString sexo,
						   QDate fecha_nacimiento)
{
	QSqlQuery consulta(*db_local);
	consulta.prepare("UPDATE personas "
					 "SET nombre=:nombre,apellido_paterno=:apellido_paterno,apellido_materno=:apellido_materno,sexo=:sexo,fecha_nacimiento=:fecha_nacimiento"
					 "WHERE rut=:rut");
	consulta.bindValue(":rut", rut);
	consulta.bindValue(":nombre", nombre);
	consulta.bindValue(":apellido_paterno", apellido_paterno);
	consulta.bindValue(":apellido_materno", apellido_materno);
	consulta.bindValue(":sexo", sexo);
	consulta.bindValue(":fecha_nacimiento", fecha_nacimiento);
	return consulta.exec();
}

bool db::persona_buscar(QString rut)
{
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT rut "
					 "FROM personas "
					 "WHERE rut=:rut");
	consulta.bindValue(":rut", rut);
	return ( consulta.exec() && consulta.next() );
}

QString db::persona_nombrar(QString rut)
{
	QString descripcion = "";
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT nombre||' '||apellido_paterno||' '||apellido_materno AS descripcion "
					 "FROM personas "
					 "WHERE rut=:rut");
	consulta.bindValue(":rut", rut);
	if ( consulta.exec() && consulta.next() )
		descripcion = consulta.record().value(0).toString();
	return descripcion;
}

QStringList db::persona_datos(QString rut)
{
	QStringList datos;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT nombre,apellido_paterno,apellido_materno,sexo,fecha_nacimiento "
					 "FROM personas "
					 "WHERE rut=:rut");
	consulta.bindValue(":rut", rut);
	if ( !consulta.exec() || !consulta.next() )
		return datos;
	for ( int i = 0; i < 5; i++ )
		datos.append( consulta.record().value(i).toString() );
	return datos;
}

QStringList db::cursos_anios()
{
	QStringList anios;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT anio "
					 "FROM cursos "
					 "GROUP BY anio");
	if ( !consulta.exec() )
		return anios;
	while ( consulta.next() )
		anios.append( consulta.record().value(0).toString() );
	return anios;
}

QStringList db::cursos_tipos(QString anio)
{
	QStringList tipos;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT tipo "
					 "FROM cursos "
					 "WHERE anio=:anio "
					 "GROUP BY tipo");
	consulta.bindValue(":anio", anio);
	if ( !consulta.exec() )
		return tipos;
	while ( consulta.next() )
		tipos.append( consulta.record().value(0).toString() );
	return tipos;
}

QStringList db::cursos_nivel(QString anio, QString tipo)
{
	QStringList niveles;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT nivel "
					 "FROM cursos "
					 "WHERE anio=:anio AND tipo=:tipo "
					 "GROUP BY nivel "
					 "ORDER BY nivel");
	consulta.bindValue(":anio", anio);
	consulta.bindValue(":tipo", tipo);
	if ( !consulta.exec() )
		return niveles;
	while ( consulta.next() )
		niveles.append( consulta.record().value(0).toString() );
	return niveles;
}

QStringList db::cursos_letras(QString anio, QString tipo, QString nivel)
{
	QStringList letras;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT letra "
					 "FROM cursos "
					 "WHERE anio=:anio AND tipo=:tipo AND nivel=:nivel "
					 "GROUP BY letra "
					 "ORDER BY letra");
	consulta.bindValue(":anio", anio);
	consulta.bindValue(":tipo", tipo);
	consulta.bindValue(":nivel", nivel);
	if ( !consulta.exec() )
		return letras;
	while ( consulta.next() )
		letras.append( consulta.record().value(0).toString() );
	return letras;
}

QString db::cursos_id(QString anio, QString tipo, QString nivel, QString letra)
{
	QString id = "";
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT id_curso "
					 "FROM cursos "
					 "WHERE anio=:anio AND tipo=:tipo AND nivel=:nivel AND letra=:letra");
	consulta.bindValue(":anio", anio);
	consulta.bindValue(":tipo", tipo);
	consulta.bindValue(":nivel", nivel);
	consulta.bindValue(":letra", letra);
	if ( !consulta.exec() || !consulta.next())
		return id;
	return consulta.record().value(0).toString();
}

bool db::cursos_agregar(QString anio,
						QString tipo,
						QString nivel,
						QString letra,
						QString rut_profesor,
						QString plan,
						QString descripcion)
{
	QSqlQuery consulta(*db_local);
	if ( cursos_id(anio, tipo, nivel, letra) != 0 )
		return false;
	consulta.prepare("INSERT INTO cursos "
					 "(id_curso,anio,tipo,nivel,letra,rut_profesor_jefe,plan,descripcion) "
					 "VALUES "
					 "(:id_curso,:anio,:tipo,:nivel,:letra,:rut_profesor_jefe,:plan,:descripcion)");
	consulta.bindValue(":id_curso", cursos_id(anio, tipo, nivel, letra) );
	consulta.bindValue(":anio", anio);
	consulta.bindValue(":tipo", tipo);
	consulta.bindValue(":nivel", nivel);
	consulta.bindValue(":letra", letra);
	consulta.bindValue(":rut_profesor_jefe", rut_profesor);
	consulta.bindValue(":plan", plan);
	consulta.bindValue(":descripcion", descripcion);
//	QString ejecutada = consulta.lastQuery();
	return consulta.exec();
}

QStringList db::cursos_datos(QString id){
	QStringList datos;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT anio,tipo,nivel,letra,rut_profesor_jefe,plan,descripcion "
					 "FROM cursos "
					 "WHERE id_curso=:id");
	consulta.bindValue(":id", id);
	if ( !consulta.exec() || !consulta.next() )
		return datos;
	for ( int i = 0; i < 6; i++ )
		datos.append( consulta.record().value(i).toString() );
	return datos;
}

bool db::alumo_en_curso(QString rut,
						QString curso)
{
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT id "
					 "FROM alumno_en_curso "
					 "WHERE rut_alumno=:rut AND id_curso=:curso");
	consulta.bindValue(":rut", rut);
	consulta.bindValue(":curso", curso);
	if ( !consulta.exec() || !consulta.next() )
		return false;
	return true;
}

bool db::alumno_agregar(QString rut,
						QString curso)
{
	QSqlQuery consulta(*db_local);
	if ( alumo_en_curso(rut, curso) )
		consulta.prepare("UPDATE alumno_en_curso "
						 "SET vigente=1 "
						 "WHERE rut_alumno=:rut AND id_curso=:curso");
	else
		consulta.prepare("INSERT INTO alumno_en_curso "
						 "(rut_alumno, id_curso, vigente) "
						 "VALUES (:rut, :curso, 1)");
	consulta.bindValue(":rut", rut);
	consulta.bindValue(":curso", curso);
	return consulta.exec();
}

bool db::personas_agregar_muchas(QString datos)
{
	QSqlQuery consulta(*db_local);
	datos.truncate( datos.length() - 1 );
	consulta.prepare( "INSERT OR IGNORE INTO temporal "
					  "VALUES " + datos + ";");
	return consulta.exec();
}

bool db::cursos_agregar_batch(QString datos)
{
	QSqlQuery consulta(*db_local);
	datos.truncate( datos.length() - 1 );
	consulta.prepare( "INSERT OR IGNORE INTO cursos "
					  "(anio,tipo,nivel,letra,descripcion) "
					  "VALUES " + datos + ";" );
	return consulta.exec();
}


bool db::alumno_quitar(QString rut,
					   QString curso)
{
	QSqlQuery consulta(*db_local);
	if ( !alumo_en_curso(rut, curso) )
		return false;
	consulta.prepare("UPDATE alumno_en_curso "
					 "SET vigente=0 "
					 "WHERE rut_alumno=:rut AND id_curso=:curso");
	consulta.bindValue(":rut", rut);
	consulta.bindValue(":curso", curso);
	return consulta.exec();
}

QString db::profesor_curso(QString curso_id)
{
	QString prof = "";
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT nombre||' '||apellido_paterno||' '||apellido_materno "
					 "FROM personas,cursos "
					 "WHERE id_curso=:id_curso "
					 "AND personas.rut=cursos.rut_profesor_jefe");
	consulta.bindValue(":id_curso", curso_id);
	if ( !consulta.exec() || !consulta.next())
		return prof;
	return consulta.record().value(0).toString();
}

void db::consolidar_temporal()
{
	QSqlQuery consulta(*db_local);
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

QString db::id_asignatura(QString id_curso, QString id_codigo)
{
	QString id = "";
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT id_asignatura "
					 "FROM asignaturas "
					 "WHERE id_curso=:id_curso AND id_codigo=:id_codigo");
	consulta.bindValue(":id_curso", id_curso);
	consulta.bindValue(":id_codigo", id_codigo);
	if ( !consulta.exec() || !consulta.next())
		return id;
	return consulta.record().value(0).toString();
}

QString db::asignatura_desc(QString id_asignatura)
{
	QString id = "";
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT codigos_asignaturas.subsector "
					 "FROM codigos_asignaturas,asignaturas "
					 "WHERE asignaturas.id_asignatura=:id_asignatura "
					 "AND asignaturas.id_codigo=codigos_asignaturas.id;");
	consulta.bindValue(":id_asignatura", id_asignatura);
	if ( !consulta.exec() || !consulta.next())
		return id;
	return consulta.record().value(0).toString();
}

QStringList db::asignaturas_curso(QString id_curso)
{
	QStringList asignaturas;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT id_asignatura "
					 "FROM asignaturas "
					 "WHERE id_curso=:id_curso;");
	consulta.bindValue(":id_curso", id_curso);
	if ( !consulta.exec())
		return asignaturas;
	while (consulta.next())
		asignaturas.append(consulta.record().value(0).toString());
	return asignaturas;
}

bool db::agregar_asignatura(QString id_curso,
                            QString rut_profesor,
                            QString id_codigo,
                            QString ponderable)
{
    QSqlQuery consulta(*db_local);
    if ( id_asignatura(id_curso, id_codigo).compare("") != 0 )
        return false;
    else
        consulta.prepare("INSERT INTO asignaturas "
                         "(id_curso,rut_profesor,id_codigo,ponderable) "
                         "VALUES "
                         "(:id_curso,:rut_profesor,:id_codigo,:ponderable)");
    consulta.bindValue(":id_curso", id_curso);
    consulta.bindValue(":rut_profesor", rut_profesor);
    consulta.bindValue(":id_codigo", id_codigo);
    consulta.bindValue(":ponderable", ponderable);
    return consulta.exec();
}

bool db::prueba_agregar(QString id_asignatura,
						QString periodo,
						QString coeficiente,
						QString valida,
						QString descripcion)
{
	QSqlQuery consulta(*db_local);
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
	QString id_prueba = prueba_id( id_asignatura,
								   periodo,
								   coeficiente,
								   valida,
								   descripcion);
	return prueba_agregar_a_curso(id_asignatura, id_prueba);
}

bool db::prueba_agregar_a_curso(QString id_asignatura, QString id_prueba)
{
	QSqlQuery consulta(*db_local);
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
	//consulta.finish();
	return consulta.exec(datos);
	//return consulta.exec("INSERT INTO notas (id_prueba,rut_alumno,nota) VALUES " + datos);
}

QString db::prueba_id(QString id_asignatura,
						QString periodo,
						QString coeficiente,
						QString valida,
						QString descripcion)
{
	QString id = "";
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT id_prueba "
					 "FROM pruebas "
					 "WHERE id_asignatura=:id_asignatura "
					 "AND periodo=:periodo "
					 "AND coeficiente=:coeficiente "
					 "AND valida=:valida "
					 "AND descripcion=:descripcion");
	consulta.bindValue(":id_asignatura", id_asignatura);
	consulta.bindValue(":periodo", periodo);
	consulta.bindValue(":coeficiente", coeficiente);
	consulta.bindValue(":valida", valida);
	consulta.bindValue(":descripcion", descripcion);
	if ( !consulta.exec() || !consulta.next())
		return id;
	return consulta.record().value(0).toString();
}

QStringList db::periodo_obtener(QString id_asignatura)
{
	QStringList id;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT periodo "
					 "FROM pruebas "
					 "WHERE id_asignatura=:id_asignatura "
					 "GROUP BY periodo "
					 "ORDER BY periodo");
	consulta.bindValue(":id_asignatura", id_asignatura);
	if ( !consulta.exec())
		return id;
	while (consulta.next())
		id.append(consulta.record().value(0).toString());
	return id;
}

QStringList db::periodo_de_curso(QString id_curso)
{
	QStringList id;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT periodo "
					 "FROM pruebas "
					 "WHERE id_asignatura IN "
					 "(SELECT asignaturas.id_asignatura "
					 "FROM asignaturas "
					 "WHERE asignaturas.id_curso=:id_curso) "
					 "GROUP BY periodo "
					 "ORDER BY periodo;");
	consulta.bindValue(":id_curso", id_curso);
	if ( !consulta.exec())
		return id;
	while (consulta.next())
		id.append(consulta.record().value(0).toString());
	return id;
}

QStringList db::notas_asignatura_rut(QString id_asignatura, QString rut_alumno)
{
	QStringList id;
	QSqlQuery consulta(*db_local);
	consulta.prepare("SELECT notas.nota "
					 "FROM notas "
					 "WHERE notas.rut_alumno=:rut_alumno AND id_prueba IN "
					 "(SELECT pruebas.id_prueba "
					 "FROM pruebas "
					 "WHERE pruebas.id_asignatura=:id_asignatura);");
	consulta.bindValue(":id_asignatura", id_asignatura);
	consulta.bindValue(":rut_alumno", rut_alumno);
	if ( !consulta.exec())
		return id;
	while (consulta.next())
		id.append(QString::number(consulta.record().value(0).toFloat(), '.', 1));
	return id;
}
