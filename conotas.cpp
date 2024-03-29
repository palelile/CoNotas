#include "conotas.h"
#include "ui_conotas.h"

CoNotas::CoNotas(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CoNotas)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    mi_db = new dbColegio();
    mi_db->conectar("colegio.db");
    ui->setupUi(this);
	con_val_rut();	// Validadores de RUT
	ini_cam_libres();

	main_signals_slots();
    ui->toolBox->setCurrentIndex(0);
    ui->toolBox->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(0);
    cargarOpciones();

}

CoNotas::~CoNotas()
{
    guardarOpciones();
    delete ui;
}

void CoNotas::main_signals_slots()
{
	// Globales
	connect(ui->tabWidget, SIGNAL(currentChanged(int)),
			this, SLOT(administracion_activa(int)) );
	connect(ui->toolBox, SIGNAL(currentChanged(int)),
			this, SLOT(pag_activa(int)) );
	// Informes
	connect(ui->inf_anio, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(inf_cargar_tipo(QString)) );
	connect(ui->inf_tipo, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(inf_cargar_nivel(QString)) );
	connect(ui->inf_nivel, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(inf_cargar_letra(QString)) );
	connect(ui->inf_letra, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(inf_cargar_periodo(QString)) );
	connect(ui->inf_letra, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(inf_cargar_alumnos(QString)) );
	connect(ui->bot_inf_parcial, SIGNAL(pressed()),
			this, SLOT(inf_parc()) );
	connect(ui->bot_inf_semestral, SIGNAL(pressed()),
			this, SLOT(inf_sem()) );
	// Administracion >> Personas
	connect(ui->rut, SIGNAL(returnPressed()),
			ui->digito, SLOT(setFocus()));
	connect(ui->digito, SIGNAL(returnPressed()),
			ui->sexo, SLOT(setFocus()));
	connect(ui->nombres, SIGNAL(returnPressed()),
			ui->apellido_paterno, SLOT(setFocus()));
	connect(ui->apellido_paterno, SIGNAL(returnPressed()),
			ui->apellido_materno, SLOT(setFocus()));
	connect(ui->apellido_materno, SIGNAL(returnPressed()),
			ui->boton_agregar_modificar, SLOT(setFocus()));
	connect(ui->boton_agregar_modificar, SIGNAL(pressed()),
			this, SLOT(adm_per_agr_mod()));
	// Administración >> Cursos
	connect(ui->rut_profesor_jefe, SIGNAL(textChanged(QString)),
			this, SLOT(rut_mod_profesor_jefe()));
	connect(ui->rut_alumno, SIGNAL(textChanged(QString)),
			this, SLOT(cur_rut_alumno()));
	connect(ui->boton_agregar_curso, SIGNAL(pressed()),
			this, SLOT(cur_curso_agregar()));
	connect(ui->exis_anio, SIGNAL(currentIndexChanged(int)),
			this, SLOT(cur_tipos_cargar()));
	connect(ui->exis_tipo, SIGNAL(currentIndexChanged(int)),
			this, SLOT(cur_niveles_cargar()));
	connect(ui->exis_nivel, SIGNAL(currentIndexChanged(int)),
			this, SLOT(cur_letras_cargar()));
	connect(ui->exis_letra, SIGNAL(currentIndexChanged(int)),
			this, SLOT(cur_alumnos_cargar()));
	connect(ui->boton_agregar_alumno, SIGNAL(pressed()),
			this, SLOT(cur_alumno_agregar()));
	// Administración >> Asignaturas
	connect(ui->asig_anio, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(asig_cargar_tipo(QString)));
	connect(ui->asig_tipo, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(asig_cargar_nivel(QString)));
	connect(ui->asig_nivel, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(asig_cargar_letra(QString)));
	connect(ui->asig_letra, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(asig_cargar_disponibles()));
	connect(ui->asig_letra, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(asig_poblar_tabla(QString)));
	connect(ui->asig_rut, SIGNAL(textChanged(QString)),
				this, SLOT(asig_alumno_rut_info()));
	connect(ui->asig_periodo_filtro, SIGNAL(currentIndexChanged(QString)),
				this, SLOT(prueb_cargar_de_periodo()));
	connect(ui->asig_periodo_asig, SIGNAL(currentIndexChanged(QString)),
				this, SLOT(prueb_poblar_notas()));
	connect(ui->asig_agregar, SIGNAL(pressed()),
				this, SLOT(asig_agregar()));
	// Administración >> Importar
	connect(ui->importar_sige, SIGNAL(pressed()),
				this, SLOT(importar_Sige()));
	// Pruebas y notas
	connect(ui->prueb_anio, SIGNAL(currentIndexChanged(QString)),
				this, SLOT(prueb_cargar_tipo(QString)));
	connect(ui->prueb_tipo, SIGNAL(currentIndexChanged(QString)),
				this, SLOT(prueb_cargar_nivel(QString)));
	connect(ui->prueb_nivel, SIGNAL(currentIndexChanged(QString)),
				this, SLOT(prueb_cargar_letra(QString)));
	connect(ui->prueb_letra, SIGNAL(currentIndexChanged(QString)),
				this, SLOT(prueb_asig_vincular_disponibles()));
	connect(ui->prueb_asig, SIGNAL(currentIndexChanged(QString)),
				this, SLOT(asig_per_vincular_disponibles()));
	connect(ui->prueb_agregar, SIGNAL(pressed()),
				this, SLOT(prueb_agregar()));
}

/******************************************************************************/

void CoNotas::administracion_activa(int actual)
{
	switch ( actual )
	{
		case 0:	// Personas
			adm_per_tab_vin();
			break;
		case 1: // Cursos
			cur_anios_cargar();
			break;
		case 2: // Asignaturas
			asig_cargar_anio();
			asig_cargar_tipo(ui->asig_anio->currentText());
			asig_cargar_nivel(ui->asig_tipo->currentText());
			asig_cargar_letra(ui->asig_nivel->currentText());
			break;
		case 3:	// Importar
			break;
	}
}

void CoNotas::pag_activa(int actual)
{
	switch ( actual )
	{
		case 0:	// Administracion
			ui->tabWidget->setCurrentIndex(1);
			ui->tabWidget->setCurrentIndex(0);
			break;
		case 1: // Pruebas
			prueb_cargar_anio();
			break;
		case 2: // Informes
			inf_cargar_anio();
			break;
	}
}

/******************************************************************************/


/*******************************************************************************
 * Inicializa los campos de la GUI que no dependen de factores
 ******************************************************************************/
void CoNotas::ini_cam_libres()
{
	ui->anio->setValue( QDate::currentDate().year() );
}

/*******************************************************************************
 * Crea los validadores de RUT
 ******************************************************************************/
void CoNotas::con_val_rut()
{
	QRegExp eval_rut("[0-9]\\d{0,8}");
	QValidator *val_rut = new QRegExpValidator(eval_rut, this);
	ui->rut->setValidator(val_rut);

	QRegExp eval_digito("[kK0-9]\\d{1}");
	QValidator *val_digito = new QRegExpValidator(eval_digito, this);
	ui->digito->setValidator(val_digito);

	QRegExp eval_rut_docente("[0-9]\\d{0,8}");
	QValidator *val_rut_docente = new QRegExpValidator(eval_rut_docente, this);
	ui->rut_profesor_jefe->setValidator(val_rut_docente);

	QRegExp eval_rut_alumno("[0-9]\\d{0,8}");
	QValidator *val_rut_alumno = new QRegExpValidator(eval_rut_alumno, this);
	ui->rut_alumno->setValidator(val_rut_alumno);
}




/*******************************************************************************
 * Adminstración de personas
 ******************************************************************************/




/*******************************************************************************
 * Vincula la tabla personas al Administrador de Personas
 ******************************************************************************/
void CoNotas::adm_per_tab_vin()
{
	if ( ui->tabla_personas->model() != NULL )
		return;
    QSqlRelationalTableModel * modelo = mi_db->modelo(this);
	modelo->setTable("personas");
	modelo->setRelation(4, QSqlRelation("sexos", "id_sexo", "descripcion"));
	modelo->setEditStrategy(QSqlTableModel::OnFieldChange);
	modelo->select();
	modelo->setHeaderData(0, Qt::Horizontal, "Rut");
	modelo->setHeaderData(1, Qt::Horizontal, "Nombre");
	modelo->setHeaderData(2, Qt::Horizontal, "Apellido Paterno");
	modelo->setHeaderData(3, Qt::Horizontal, "Apellido Materno");
	modelo->setHeaderData(4, Qt::Horizontal, "Sexo");
	modelo->setHeaderData(5, Qt::Horizontal, "Fecha de Nacimiento");
	ui->tabla_personas->setModel( modelo );
	ui->tabla_personas->setItemDelegate(new QSqlRelationalDelegate(ui->tabla_personas));
	ui->tabla_personas->resizeColumnsToContents();
}

/*******************************************************************************
 * Agregar o modificar personas
 ******************************************************************************/
void CoNotas::adm_per_agr_mod()
{
	if ( !rut::validar( ui->rut->text(), ui->digito->text() ) )
        return;
    QString valores = "'" + ui->rut->text() + "','" +
            ui->nombres->text() + "','" +
            ui->apellido_paterno->text() + "','" +
            ui->apellido_materno->text() + "','" +
            ui->sexo->currentText() + "','" +
            ui->fecha_nacimiento->date().toString("yyyy-MM-dd") + "'";
//	if ( mi_db->persona_buscar( ui->rut->text() ))
    if ( mi_db->buscar( "rut", "personas", "rut=" + ui->rut->text() ))
        mi_db->modificar("personas",
                         "rut=" + ui->rut->text(),
                         "rut,nombre,apellido_paterno,apellido_materno,sexo,fecha_nacimiento",
                         valores);
    else
        mi_db->agregar("personas",
                       "rut,nombre,apellido_paterno,apellido_materno,sexo,fecha_nacimiento",
                       valores);
	((QSqlRelationalTableModel *)ui->tabla_personas->model())->select();
}




/*******************************************************************************
 * Adminstración de cursos
 ******************************************************************************/



/*******************************************************************************
 * Muestra los datos asiciados al rut ingresado
 ******************************************************************************/
void CoNotas::rut_mod_profesor_jefe()
{
    QStringList * persona = mi_db->buscar("nombre||' '||apellido_paterno||' '||apellido_materno AS persona",
                                          "personas",
                                          "rut='" + ui->rut_profesor_jefe->text() + "'");
    if (persona)
        ui->datos_docente->setText( persona->at(0) );
}

/*******************************************************************************
 * Construye el listado de años disponibles
 ******************************************************************************/
void CoNotas::cur_anios_cargar()
{
    QString consulta = "SELECT anio FROM cursos GROUP BY anio";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->exis_anio->setModel( modelo );
}

/*******************************************************************************
 * Construye listado de tipos para el año actual
 ******************************************************************************/
void CoNotas::cur_tipos_cargar()
{
    QString consulta = "SELECT tipo FROM cursos "
            "WHERE anio=" + ui->exis_anio->currentText() +
            " GROUP BY tipo";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->exis_tipo->setModel( modelo );
}

/*******************************************************************************
 * Construye listado de niveles para el año y tipo actual
 ******************************************************************************/
void CoNotas::cur_niveles_cargar()
{
    QString consulta = "SELECT nivel FROM cursos "
            " WHERE anio='" + ui->exis_anio->currentText() +
            "' AND tipo='" + ui->exis_tipo->currentText() +
            "' GROUP BY nivel ORDER BY nivel";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->exis_nivel->setModel( modelo );
}

/*******************************************************************************
 * Construye listado de letras para el año, tipo, y nivel actual
 ******************************************************************************/
void CoNotas::cur_letras_cargar()
{
    QString consulta = "SELECT letra FROM cursos "
            "WHERE anio='" + ui->exis_anio->currentText() +
            "' AND tipo='" + ui->exis_tipo->currentText() +
            "' AND nivel='" + ui->exis_nivel->currentText() +
            "' GROUP BY letra ORDER BY letra";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->exis_letra->setModel( modelo );
}

/*******************************************************************************
 * Construye listado de alumnos para el curso actual
 ******************************************************************************/
void CoNotas::cur_alumnos_cargar()
{
	QString curso_id = mi_db->cursos_id(
						   ui->exis_anio->currentText(),
						   ui->exis_tipo->currentText(),
						   ui->exis_nivel->currentText(),
						   ui->exis_letra->currentText()
						   );

	QString filtro = "id_curso='" + curso_id + "'";
    QSqlRelationalTableModel * modelo = mi_db->modelo(this);
	modelo->setTable("alumno_en_curso");
	modelo->setRelation(1, QSqlRelation("personas", "rut", "nombre||' '||apellido_paterno||' '||apellido_materno AS alumno"));
	modelo->setFilter(filtro);
	modelo->setEditStrategy(QSqlTableModel::OnFieldChange);
	modelo->setHeaderData(0, Qt::Horizontal, tr("ID"));
	modelo->setHeaderData(1, Qt::Horizontal, tr("Alumno"));
	modelo->setHeaderData(2, Qt::Horizontal, tr("Curso"));
	modelo->setHeaderData(3, Qt::Horizontal, tr("Vigente"));
	modelo->select();
	ui->tabla_alumnos->setModel( modelo );
	ui->tabla_alumnos->setItemDelegate(new QSqlRelationalDelegate(ui->tabla_alumnos));
	ui->tabla_alumnos->resizeColumnsToContents();
	ui->tabla_alumnos->hideColumn(0);
	ui->tabla_alumnos->hideColumn(2);
}

/*******************************************************************************
 * Muestra información para el rut ingresado
 ******************************************************************************/
void CoNotas::cur_rut_alumno()
{
    QStringList * persona = mi_db->buscar("nombre||' '||apellido_paterno||' '||apellido_materno AS persona",
                                          "personas",
                                          "rut='" + ui->rut_alumno->text() + "'");
    if (persona)
        ui->datos_alumno->setText( persona->at(0) );
}

/*******************************************************************************
 * Agrega un alumo al curso
 ******************************************************************************/
void CoNotas::cur_alumno_agregar()
{
	QString id_curso = mi_db->cursos_id(
				ui->exis_anio->currentText(),
				ui->exis_tipo->currentText(),
				ui->exis_nivel->currentText(),
				ui->exis_letra->currentText()
				);
	mi_db->alumno_agregar(ui->rut_alumno->text(),
						  id_curso);
	((QSqlRelationalTableModel * )ui->tabla_alumnos->model())->select();
}

/*******************************************************************************
 * Agrega o modifica un curso
 ******************************************************************************/
void CoNotas::cur_curso_agregar()
{
    QString id_curso = mi_db->cursos_id(QString::number(ui->anio->value()),
                                        ui->tipo->currentText(),
                                        QString::number(ui->nivel->value()),
                                        ui->letra->currentText());
    QString valores = "'" + id_curso + "','" +
            QString::number(ui->anio->value()) + "','" +
            ui->tipo->currentText() + "','" +
            QString::number(ui->nivel->value()) + "','" +
            ui->letra->currentText() + "','" +
            ui->rut_profesor_jefe->text() + "','" +
            QString::number(ui->plan->value()) + "','" +
            ui->descripcion->text() + "'";
    if ( !mi_db->agregar("cursos",
                   "id_curso,anio,tipo,nivel,letra,rut_profesor_jefe,plan,descripcion",
                   valores) )
        mi_db->modificar("cursos",
                         "id_curso='" + id_curso + "'",
                         "id_curso,anio,tipo,nivel,letra,rut_profesor_jefe,plan,descripcion",
                         valores);

	cur_anios_cargar();
	((QSqlRelationalTableModel * )ui->tabla_alumnos->model())->select();
}




/*******************************************************************************
 * Importación desde SIGE
 ******************************************************************************/



/*******************************************************************************
 * Importa el archivo CSV creaso por SIGE
 ******************************************************************************/
void CoNotas::importar_Sige()
{
    QString archivo = QFileDialog::getOpenFileName(
                this,
                "Abrir archivo",
                QDir::currentPath(),
                "Archivo TXT de SIGE (*.txt)" );
	if( archivo.isNull() )
		ui->statusBar->showMessage("Archivo no seleccionado", 1000);
	else
	{
		csv_lector csv;
		csv.leer_archivo_db( archivo, mi_db );
	}
}




/*******************************************************************************
 * Adminstración de asignaturas
 ******************************************************************************/



/*******************************************************************************
 * Carga el listado de años disponibles
 ******************************************************************************/
void CoNotas::asig_cargar_anio()
{
    QString consulta = "SELECT anio FROM cursos GROUP BY anio";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->asig_anio->setModel( modelo );
}

/*******************************************************************************
 * Carga el listado de tipos disponibles para el año seleccionado
 ******************************************************************************/
void CoNotas::asig_cargar_tipo(QString anio)
{
    QString consulta = "SELECT tipo FROM cursos "
            "WHERE anio=" + anio +
            " GROUP BY tipo";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->asig_tipo->setModel( modelo );
}

/*******************************************************************************
 * Carga el listado de niveles disponibles para el año y tipo seleccionado
 ******************************************************************************/
void CoNotas::asig_cargar_nivel( QString tipo)
{
    QString consulta = "SELECT nivel FROM cursos "
            " WHERE anio='" + ui->asig_anio->currentText() +
            "' AND tipo='" + tipo +
            "' GROUP BY nivel ORDER BY nivel";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->asig_nivel->setModel( modelo );
}

/*******************************************************************************
 * Carga el listado de letras disponibles para el nivel, tipo y año seleccionado
 ******************************************************************************/
void CoNotas::asig_cargar_letra(QString nivel)
{
    QString consulta = "SELECT letra FROM cursos "
            "WHERE anio='" + ui->asig_anio->currentText() +
            "' AND tipo='" + ui->asig_tipo->currentText() +
            "' AND nivel='" + nivel +
            "' GROUP BY letra ORDER BY letra";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->asig_letra->setModel( modelo );
}

/*******************************************************************************
 * Carga el listado de asignaturas disponibles
 ******************************************************************************/
void CoNotas::asig_cargar_disponibles()
{
	QString curso_id = mi_db->cursos_id(
				ui->asig_anio->currentText(),
				ui->asig_tipo->currentText(),
				ui->asig_nivel->currentText(),
				ui->asig_letra->currentText()
				);
	QString consulta = "SELECT codigos_asignaturas.subsector,codigos_asignaturas.id "
					   "FROM decretos,codigos_asignaturas "
					   "WHERE codigos_asignaturas.id_decreto=decretos.id "
					   "AND decretos.tipo_ensenanza='" + ui->asig_tipo->currentText() + "' "
					   "AND decretos.nivel='" + ui->asig_nivel->currentText() +
					   "' AND codigos_asignaturas.id NOT IN "
					   "(SELECT id_codigo FROM asignaturas WHERE id_curso='" + curso_id + "');";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
	modelo->setHeaderData(0, Qt::Horizontal, tr("ID"));
	modelo->setHeaderData(1, Qt::Horizontal, tr("SubSector"));
	ui->asig_asig->setModel( modelo );
}

/*******************************************************************************
 * Poblar la tabla con las asignaturas del curso
 ******************************************************************************/
void CoNotas::asig_poblar_tabla(QString letra)
{
	QString curso_id = mi_db->cursos_id(
				ui->asig_anio->currentText(),
				ui->asig_tipo->currentText(),
				ui->asig_nivel->currentText(),
				letra
				);

	QString filtro = "id_curso='" + curso_id + "'";
    QSqlRelationalTableModel * modelo = mi_db->modelo(this);
	modelo->setTable("asignaturas");
	modelo->setRelation(2, QSqlRelation("personas",
										"rut",
										"nombre||' '||apellido_paterno AS profesor"));
	modelo->setRelation(3, QSqlRelation("codigos_asignaturas",
										"id",
										"subsector"));
	modelo->setFilter(filtro);
	modelo->setEditStrategy(QSqlTableModel::OnFieldChange);
	modelo->setHeaderData(0, Qt::Horizontal, tr("ID"));
	modelo->setHeaderData(1, Qt::Horizontal, tr("Curso"));
	modelo->setHeaderData(2, Qt::Horizontal, tr("Profesor"));
	modelo->setHeaderData(3, Qt::Horizontal, tr("Asignatura"));
	modelo->setHeaderData(4, Qt::Horizontal, tr("Ponderable"));
	modelo->select();
	ui->tabla_asignaturas->setModel( modelo );
	ui->tabla_asignaturas->setItemDelegate(new QSqlRelationalDelegate(ui->tabla_asignaturas));
	ui->tabla_asignaturas->resizeColumnsToContents();
	ui->tabla_asignaturas->hideColumn(0);
	ui->tabla_asignaturas->hideColumn(1);
}

/*******************************************************************************
 * Agrega una asignatura al curso y actualiza el listado de disponibles
 ******************************************************************************/
void CoNotas::asig_agregar()
{
	QSqlQueryModel * modelo = ( (QSqlQueryModel * )(ui->asig_asig->model()) );
	QString id_codigo = modelo->record( ui->asig_asig->currentIndex() ).value(1).toString() ;
	QString rut_profesor = ui->asig_rut->text();
	QString ponderable = QString::number(ui->asig_ponderable->isChecked());
	QString id_curso = mi_db->cursos_id(ui->asig_anio->currentText(),
										ui->asig_tipo->currentText(),
										ui->asig_nivel->currentText(),
										ui->asig_letra->currentText());
    mi_db->agregar("asignaturas",
                   "id_curso,rut_profesor,id_codigo,ponderable",
                   "'"+id_curso+"','"+rut_profesor+"','"+id_codigo+"','"+ponderable+"'");
	((QSqlRelationalTableModel *)ui->tabla_asignaturas->model())->select();
	asig_cargar_disponibles();
}

/*******************************************************************************
 * Muestra información del rut del alumno
 ******************************************************************************/
void CoNotas::asig_alumno_rut_info()
{
    QStringList * persona = mi_db->buscar("nombre||' '||apellido_paterno||' '||apellido_materno AS persona",
                                          "personas",
                                          "rut='" + ui->asig_rut->text() + "'");
    if (persona)
        ui->datos_docente_asignatura->setText( persona->at(0) );
//	ui->datos_docente_asignatura->setText(
//				mi_db->persona_nombrar(
//					ui->asig_rut->text()));
}




/*******************************************************************************
 * Manejo de pruebas y notas
 ******************************************************************************/




/*******************************************************************************
 * Carga el listado de años disponibles
 ******************************************************************************/
void CoNotas::prueb_cargar_anio()
{
    QString consulta = "SELECT anio FROM cursos GROUP BY anio";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->prueb_anio->setModel( modelo );
}

/*******************************************************************************
 * Carga el listado de tipos disponibles para en año seleccionado
 ******************************************************************************/
void CoNotas::prueb_cargar_tipo(QString anio)
{
    QString consulta = "SELECT tipo FROM cursos "
            "WHERE anio=" + anio +
            " GROUP BY tipo";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->prueb_tipo->setModel( modelo );
}

/*******************************************************************************
 * Carga el listado de niveles disponibles para el año y tipo seleccionado
 ******************************************************************************/
void CoNotas::prueb_cargar_nivel( QString tipo)
{
    QString consulta = "SELECT nivel FROM cursos "
            " WHERE anio='" + ui->prueb_anio->currentText() +
            "' AND tipo='" + tipo +
            "' GROUP BY nivel ORDER BY nivel";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->prueb_nivel->setModel( modelo );
}

/*******************************************************************************
 * Carga el listado de letras disponibles para el nivel seleccionado
 ******************************************************************************/
void CoNotas::prueb_cargar_letra(QString nivel)
{
    QString consulta = "SELECT letra FROM cursos "
            "WHERE anio='" + ui->prueb_anio->currentText() +
            "' AND tipo='" + ui->prueb_tipo->currentText() +
            "' AND nivel='" + nivel +
            "' GROUP BY letra ORDER BY letra";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->prueb_letra->setModel( modelo );

	prueb_asig_vincular_disponibles();
}

/*******************************************************************************
 * Carga el listado de asignaturas disponibles para el curso respectivo
 ******************************************************************************/
void CoNotas::prueb_asig_vincular_disponibles()
{
	QString curso_id = mi_db->cursos_id(
				ui->prueb_anio->currentText(),
				ui->prueb_tipo->currentText(),
				ui->prueb_nivel->currentText(),
				ui->prueb_letra->currentText()
				);
	QString consulta = "SELECT codigos_asignaturas.subsector,asignaturas.id_asignatura "
					   "FROM asignaturas,codigos_asignaturas "
					   "WHERE codigos_asignaturas.id=asignaturas.id_codigo "
					   "AND asignaturas.id_curso='" + curso_id + "';";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
	modelo->setHeaderData(0, Qt::Horizontal, tr("SubSector"));
	modelo->setHeaderData(1, Qt::Horizontal, tr("ID"));
	ui->prueb_asig->setModel( modelo );
}

/*******************************************************************************
 * Carga el listado de asignaturas disponibles para el curso respectivo
 ******************************************************************************/
void CoNotas::asig_per_vincular_disponibles()
{

	QSqlQueryModel * modelo_asignatura = ( (QSqlQueryModel * )(ui->prueb_asig->model()) );
	QString asig_id = modelo_asignatura->record(ui->prueb_asig->currentIndex()).value(1).toString();
    QString consulta = "SELECT periodo "
					   "FROM pruebas "
					   "WHERE id_asignatura=" + asig_id +
					   " GROUP BY periodo "
					   "ORDER BY periodo";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
	modelo->setHeaderData(0, Qt::Horizontal, tr("Periodo"));
	ui->asig_periodo_filtro->setModel( modelo );
}

/*******************************************************************************
 * Agrega una prueba nueva
 ******************************************************************************/
void CoNotas::prueb_agregar()
{
	QSqlQueryModel * modelo = ( (QSqlQueryModel * )(ui->prueb_asig->model()) );
	QString id_asignatura = modelo->record( ui->prueb_asig->currentIndex() ).value(1).toString() ;
	mi_db->prueba_agregar(id_asignatura,
						  QString::number(ui->prueb_periodo->value()),
						  QString::number(ui->prueb_coeficiente->value()),
						  "1",
						  ui->prueb_descripcion->text());
	asig_per_vincular_disponibles();
}

/*******************************************************************************
 * Cargar pruebas del periodo
 ******************************************************************************/
void CoNotas::prueb_cargar_de_periodo()
{
	QSqlQueryModel * mod_asig = ( (QSqlQueryModel * )(ui->prueb_asig->model()) );
	QString asig_id = mod_asig->record(ui->prueb_asig->currentIndex()).value(1).toString();

	QString consulta = "SELECT descripcion,id_prueba "
					   "FROM pruebas "
					   "WHERE id_asignatura=" + asig_id +
					   " AND periodo=" + ui->asig_periodo_filtro->currentText() +
					   " ORDER BY id_prueba";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
	modelo->setHeaderData(0, Qt::Horizontal, tr("prueba"));
	modelo->setHeaderData(1, Qt::Horizontal, tr("id"));
	ui->asig_periodo_asig->setModel( modelo );
}

/*******************************************************************************
 * Se puebla la tabla de notas
 ******************************************************************************/
void CoNotas::prueb_poblar_notas()
{

	QSqlQueryModel * mod_pru = ( (QSqlQueryModel * )(ui->asig_periodo_asig->model()) );
	QString prueba_id = mod_pru->record(ui->asig_periodo_asig->currentIndex()).value(1).toString();
    QSqlRelationalTableModel * modelo = mi_db->modelo(this);
	modelo->setTable("notas");
	modelo->setFilter("id_prueba='" + prueba_id + "'");
	modelo->setRelation(2, QSqlRelation("personas", "rut", "nombre"));
	modelo->filter();
	modelo->setHeaderData(0, Qt::Horizontal, tr("ID_Nota"));
	modelo->setHeaderData(1, Qt::Horizontal, tr("ID_Prueba"));
	modelo->setHeaderData(2, Qt::Horizontal, tr("Alumno"));
	modelo->setHeaderData(3, Qt::Horizontal, tr("Nota"));
	modelo->select();
	ui->prueb_notas->setModel( modelo );
//    ui->prueb_notas->hideColumn(0);
//    ui->prueb_notas->hideColumn(1);
    ui->prueb_notas->resizeColumnsToContents();
    ui->prueb_notas->installEventFilter( this );
}

bool CoNotas::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->prueb_notas && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Tab)
        {
            QModelIndex index = ui->prueb_notas->selectionModel()->currentIndex();
            if (!index.isValid())
                return false;
            QModelIndex ind = ui->prueb_notas->model()->index(
                        index.row()+1, index.column() );
            ui->prueb_notas->selectionModel()->setCurrentIndex(
                        ind,
                        QItemSelectionModel::SelectCurrent);
            ui->prueb_notas->edit(ind);
            return true;
        }
        else
            return false;
    }
    return false;
}




/*******************************************************************************
 * Informes de notas
 ******************************************************************************/



/*******************************************************************************
 * Imprimir informe parcial de notas
 ******************************************************************************/
void CoNotas::inf_parc()
{
	QPrinter printer(QPrinter::HighResolution);
	QString formato = "inf_parcial.xml";
	QtRPT *report = new QtRPT(this);
	report->loadReport(formato);
	QString curso_id = mi_db->cursos_id(ui->inf_anio->currentText(),
										ui->inf_tipo->currentText(),
                                        ui->inf_nivel->currentText(),
                                        ui->inf_letra->currentText());
    report->recordCount = mi_db->buscarColumna("id_asignatura",
                                               "asignaturas",
                                               "id_curso='" + curso_id + "'")->count();
    QObject::connect(report, SIGNAL(setValue(int&, QString&, QVariant&)),
                     this, SLOT(inf_parc_campos(int&, QString&, QVariant&)) );
    report->printExec();
}

/*******************************************************************************
 * Definir campos de informe parcial
 ******************************************************************************/
void CoNotas::inf_parc_campos(int &recNo, QString &paramName, QVariant &paramValue)
{
	// Titulo
	if (paramName == "titulo" )
	{
		paramValue = "Informe parcial de notas";
		return;
	}
	if (paramName == "periodo")
	{
		paramValue = ui->inf_periodo->currentText();
		return;
	}
	if (paramName == "year")
	{
		paramValue = ui->inf_anio->currentText();
		return;
	}
	if (paramName == "escuela")
	{
		paramValue = ui->dat_escuela->text();
		return;
	}
	if (paramName == "comuna")
	{
		paramValue = ui->dat_comuna->text();
		return;
	}

	QSqlQueryModel * modelo_alumnos = (QSqlQueryModel *)(ui->inf_alumnos->model());
	QString rut_alumno=modelo_alumnos->record(ui->inf_alumnos->currentIndex()).value(1).toString();
	if (paramName == "Alumno")
	{
        QStringList * persona = mi_db->buscar("nombre||' '||apellido_paterno||' '||apellido_materno AS persona",
                                              "personas",
                                              "rut='" + ui->rut_profesor_jefe->text() + "'");
        if (persona)
            paramValue = persona->at(0);
//		paramValue = mi_db->persona_nombrar(rut_alumno);
		return;
	}
	// Cabecera

	// Master
	QString curso_id = mi_db->cursos_id(ui->inf_anio->currentText(),
										ui->inf_tipo->currentText(),
										ui->inf_nivel->currentText(),
										ui->inf_letra->currentText());
    QStringList asignaturas = *mi_db->buscarColumna("id_asignatura",
                                                   "asignaturas",
                                                   "id_curso='" + curso_id + "'");
    QStringList *notas = new QStringList();
	QString notaActual = "";
	int count = 0;
	float promedio_parcial = 0.0;
	foreach (QString id_asignatura, asignaturas)
	{
        notas->append(*mi_db->buscarColumna(
                          "notas.nota",
                          "notas",
                          "notas.rut_alumno='" +rut_alumno+
                          "' AND id_prueba IN (SELECT pruebas.id_prueba FROM pruebas "
                          "WHERE pruebas.id_asignatura='"+id_asignatura+"');"
                          ));
        if ( count == recNo && notas )
		{
			if ( paramName == "Asignatura" )
			{
                paramValue = mi_db->buscar("codigos_asignaturas.subsector",
                       "codigos_asignaturas,asignaturas ",
                       "asignaturas.id_asignatura='" +id_asignatura+
                       "' AND asignaturas.id_codigo=codigos_asignaturas.id;")->at(0);
                return;
			}
			if ( paramName == "PP" )
			{
                paramValue = promedio( *notas );
				return;
			}
            for (int i = 0; i < notas->count() && i < 14; i++)
			{
				notaActual = "N" + QString::number(i+1);
				if ( paramName == notaActual )
				{
                    paramValue = notas->at(i);
					return;
				}
			}
		}
        if (notas)
        {
            promedio_parcial += promedio( *notas ) / (float)(asignaturas.count()+1);
            notas->clear();
        }
		count ++;
	}

	// Footer
	if ( paramName == "PPP" )
	{
		paramValue = QString::number(promedio_parcial, '.', 1);
		return;
	}
	// Sumario
	if ( paramName == "fecha" )
	{
		paramValue = QDate::currentDate();
		return;
	}
	if ( paramName == "profesor" )
	{

        paramValue = mi_db->buscar("nombre||' '||apellido_paterno||' '||apellido_materno",
                                   "personas,cursos",
                                   "id_curso='" + curso_id +
                                   "' AND personas.rut=cursos.rut_profesor_jefe")->at(0);
		return;
	}
}

/*******************************************************************************
 * Imprimir informe de notas del periodo
 ******************************************************************************/
void CoNotas::inf_sem()
{
	QString formato = "inf_periodo.xml";

	QString curso_id = mi_db->cursos_id(ui->inf_anio->currentText(),
										ui->inf_tipo->currentText(),
										ui->inf_nivel->currentText(),
										ui->inf_letra->currentText());
	QPrinter printer;
//	QString s = QFileDialog::getSaveFileName(
//					this,
//					"Elija el archivo donde se guardará el documento",
//					"",
//					"Archivo PDF (*.pdf)"
//					);
//	if (s.isEmpty())
//		return;
//	printer.setOutputFileName( s );
	printer.setPageSize(QPrinter::Letter);
	printer.setPaperSize(QPrinter::Letter);
	printer.setFullPage( false );
	printer.setPageMargins(20,20,20,20,QPrinter::Millimeter);
//	printer.setOutputFormat( QPrinter::PdfFormat );
	QtRPT *report;
	if ( ui->inf_cur_com->isChecked() )
	{
		for ( int i = 0; i < ui->inf_alumnos->count(); i++)
		{
			report = new QtRPT(this);
            report->loadReport(formato);
            report->recordCount = mi_db->buscarColumna("id_asignatura",
                                                       "asignaturas",
                                                       "id_curso='" + curso_id + "'")->count();
            ui->inf_alumnos->setCurrentIndex( i );
            QObject::connect(report, SIGNAL(setValue(int&, QString&, QVariant&)),
                             this, SLOT(inf_sem_campos(int&, QString&, QVariant&)));
            report->printExec(&printer);
        }
    }
	else
	{
		report = new QtRPT(this);
        report->loadReport(formato);
        report->recordCount = mi_db->buscarColumna("id_asignatura",
                                                   "asignaturas",
                                                   "id_curso='" + curso_id + "'")->count();
        QObject::connect(report, SIGNAL(setValue(int&, QString&, QVariant&)),
						 this, SLOT(inf_sem_campos(int&, QString&, QVariant&)));
        report->printExec();
	}
}

/*******************************************************************************
 * Definir campos de informe semestral
 ******************************************************************************/
void CoNotas::inf_sem_campos(int &recNo, QString &paramName, QVariant &paramValue)
{
	// Titulo
	if (paramName == "titulo" )
	{
		paramValue = "Informe de notas del periodo";
		return;
	}
	if (paramName == "periodo")
	{
		paramValue = ui->inf_periodo->currentText();
		return;
	}
	if (paramName == "year")
	{
		paramValue = ui->inf_anio->currentText();
		return;
	}
	if (paramName == "escuela")
	{
		paramValue = ui->dat_escuela->text();
		return;
	}
	if (paramName == "comuna")
	{
		paramValue = ui->dat_comuna->text();
		return;
	}

	QSqlQueryModel * modelo_alumnos = (QSqlQueryModel *)(ui->inf_alumnos->model());
	QString rut_alumno=modelo_alumnos->record(ui->inf_alumnos->currentIndex()).value(1).toString();
	if (paramName == "Alumno")
	{
        QStringList * persona = mi_db->buscar("nombre||' '||apellido_paterno||' '||apellido_materno AS persona",
                                              "personas",
                                              "rut='" + rut_alumno + "'");
        if (persona)
            paramValue = persona->at(0);
//		paramValue = mi_db->persona_nombrar(rut_alumno);
		return;
	}
	// Cabecera

	// Master
	QString curso_id = mi_db->cursos_id(ui->inf_anio->currentText(),
										ui->inf_tipo->currentText(),
										ui->inf_nivel->currentText(),
										ui->inf_letra->currentText());

    QStringList asignaturas = *mi_db->buscarColumna("id_asignatura",
                                                   "asignaturas",
                                                   "id_curso='" + curso_id + "'");
	QStringList notas;
	QString notaActual = "";
	int count = 0;
	float promedio_parcial = 0.0;
	foreach (QString id_asignatura, asignaturas)
	{
        notas = *mi_db->buscarColumna(
                    "notas.nota",
                    "notas",
                    "notas.rut_alumno='" +rut_alumno+
                    "' AND id_prueba IN (SELECT pruebas.id_prueba FROM pruebas "
                    "WHERE pruebas.id_asignatura='"+id_asignatura+"');"
                    );
		if ( count == recNo )
		{
			if ( paramName == "Asignatura" )
			{
                paramValue = mi_db->buscar("codigos_asignaturas.subsector",
                       "codigos_asignaturas,asignaturas ",
                       "asignaturas.id_asignatura='" +id_asignatura+
                       "' AND asignaturas.id_codigo=codigos_asignaturas.id;")->at(0);
				return;
			}
			if ( paramName == "PP" )
			{
				paramValue = promedio( notas );
				return;
			}
			for (int i = 0; i < notas.count() && i < 14; i++)
			{
				notaActual = "N" + QString::number(i+1);
				if ( paramName == notaActual )
				{
					paramValue = notas.at(i);
					return;
				}
			}
		}
		promedio_parcial += promedio( notas ) / (float)(asignaturas.count()+1);
		notas.clear();
		count ++;
	}

	// Footer
	if ( paramName == "PPP" )
	{
		paramValue = QString::number(promedio_parcial, '.', 1);
		return;
	}
	// Sumario
	if ( paramName == "fecha" )
	{
		paramValue = QDate::currentDate();
		return;
	}
	if ( paramName == "profesor" )
	{
        paramValue = mi_db->buscar("nombre||' '||apellido_paterno||' '||apellido_materno",
                                   "personas,cursos",
                                   "id_curso='" + curso_id +
                                   "' AND personas.rut=cursos.rut_profesor_jefe")->at(0);
		return;
	}
}


void CoNotas::inf_cargar_anio()
{
    QString consulta = "SELECT anio FROM cursos GROUP BY anio";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->inf_anio->setModel( modelo );
}

void CoNotas::inf_cargar_tipo(QString anio)
{
    QString consulta = "SELECT tipo FROM cursos "
            "WHERE anio=" + anio +
            " GROUP BY tipo";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->inf_tipo->setModel( modelo );
}

void CoNotas::inf_cargar_nivel( QString tipo)
{
    QString consulta = "SELECT nivel FROM cursos "
            " WHERE anio='" + ui->inf_anio->currentText() +
            "' AND tipo='" + tipo +
            "' GROUP BY nivel ORDER BY nivel";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->inf_nivel->setModel( modelo );
}

void CoNotas::inf_cargar_letra(QString nivel)
{
    QString consulta = "SELECT letra FROM cursos "
            "WHERE anio='" + ui->inf_anio->currentText() +
            "' AND tipo='" + ui->inf_tipo->currentText() +
            "' AND nivel='" + nivel +
            "' GROUP BY letra ORDER BY letra";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
    ui->inf_letra->setModel( modelo );
}

void CoNotas::inf_cargar_periodo(QString letra)
{
	ui->inf_periodo->clear();
	QString curso_id = mi_db->cursos_id(
				ui->inf_anio->currentText(),
				ui->inf_tipo->currentText(),
                ui->inf_nivel->currentText(),
                letra
                );

    QStringList periodos = *mi_db->buscarColumna("periodo",
                                                 "pruebas",
                                                 "id_asignatura IN (SELECT asignaturas.id_asignatura "
                                                 "FROM asignaturas "
                                                 "WHERE asignaturas.id_curso='" + curso_id +
                                                 "') GROUP BY periodo ORDER BY periodo;");
    ui->inf_periodo->addItems( periodos );
}

float CoNotas::promedio(QStringList lista)
{
    int contador = 0;
    if (lista.count()==0)
		return 0.0;
	float resultado = 0.0;
	for ( int i= 0; i < lista.count(); i++)
		if ( lista.at(i).toFloat() != 0 )
		{
			resultado += lista.at(i).toFloat();
			contador++;
		}
	return (resultado/(float)contador);
}

void CoNotas::inf_cargar_alumnos(QString letra)
{
	QString curso_id = mi_db->cursos_id(
				ui->inf_anio->currentText(),
				ui->inf_tipo->currentText(),
				ui->inf_nivel->currentText(),
				letra
				);

	QString consulta = "SELECT nombre||' '||apellido_paterno||' '||apellido_materno AS alumno,rut "
					   "FROM personas,alumno_en_curso "
					   "WHERE id_curso='" + curso_id +
					   "' AND personas.rut=alumno_en_curso.rut_alumno";
    QSqlQueryModel * modelo = mi_db->modeloConsulta(this, consulta);
	modelo->setHeaderData(0, Qt::Horizontal, tr("alumno"));
	modelo->setHeaderData(1, Qt::Horizontal, tr("rut"));
	ui->inf_alumnos->setModel( modelo );
}

// Guarda las opciones del programa de forma permanente
void CoNotas::guardarOpciones()
{
    QSettings opciones(QSettings::IniFormat, QSettings::UserScope, "Ashtaroth", "CoNotas");
    opciones.beginGroup("Datos");
    opciones.setValue("Escuela", ui->dat_escuela->text());
    opciones.setValue("Comuna", ui->dat_comuna->text());
    opciones.endGroup();
}

// Carga las opciones del programa
void CoNotas::cargarOpciones(){
    QSettings opciones(QSettings::IniFormat, QSettings::UserScope, "Ashtaroth", "CoNotas");
    opciones.beginGroup("Datos");
    ui->dat_escuela->setText( opciones.value("Escuela").toString() );
    ui->dat_comuna->setText( opciones.value("Comuna").toString() );
    opciones.endGroup();
}
