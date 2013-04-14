#ifndef CONOTAS_H
#define CONOTAS_H

#include <QMainWindow>
#include <QTextCodec>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlIndex>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QVariant>
#include <QSqlRelationalDelegate>
#include <QAbstractItemModel>
#include <QSqlRelation>
#include "dbcolegio.h"
#include "rut.h"
#include "csv_lector.h"
#include "qtrpt.h"
#include <QFileDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QSettings>
#include <QDir>
#include <QDoubleValidator>
#include <QPoint>

namespace Ui {
class CoNotas;
}

class CoNotas : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CoNotas(QWidget *parent = 0);
    ~CoNotas();
    
private:
    Ui::CoNotas *ui;
    dbColegio * mi_db;
	void main_signals_slots();
	void con_val_rut();
	float promedio(QStringList lista);
	void ini_cam_libres();
	// Manejo de personas
	void adm_per_tab_vin();
	void construir_linea_personas();
    bool eventFilter(QObject *object, QEvent *event);
	// Variables de administración de personas




private slots:
	void adm_per_agr_mod();
	void rut_mod_profesor_jefe();
	void cur_anios_cargar();
	void cur_tipos_cargar();
	void cur_niveles_cargar();
	void cur_letras_cargar();
	void cur_alumnos_cargar();
	void cur_rut_alumno();
	void cur_alumno_agregar();
	void cur_curso_agregar();
	void importar_Sige();

	// Manejadores de combobox de area de administracion de asignaturas
	void asig_cargar_anio();
	void asig_cargar_tipo(QString anio);
	void asig_cargar_nivel(QString tipo);
	void asig_cargar_letra(QString nivel);
	void asig_poblar_tabla(QString letra);
	void asig_cargar_disponibles();
	void asig_agregar();
	void asig_alumno_rut_info();
	// Complemento del manejo de pruebas
	void prueb_cargar_anio();
	void prueb_cargar_tipo(QString anio);
	void prueb_cargar_nivel(QString tipo);
	void prueb_cargar_letra(QString nivel);
	void prueb_asig_vincular_disponibles();
	void prueb_agregar();
	// Notas
	void asig_per_vincular_disponibles();
	void prueb_cargar_de_periodo();
	void prueb_poblar_notas();
	// Informes
	void inf_parc();
	void inf_parc_campos(int &recNo, QString &paramName, QVariant &paramValue);
	void inf_sem();
	void inf_sem_campos(int &recNo, QString &paramName, QVariant &paramValue);
	void inf_cargar_anio();
	void inf_cargar_tipo(QString anio);
	void inf_cargar_nivel( QString tipo);
	void inf_cargar_letra(QString nivel);
	void inf_cargar_periodo(QString letra);
	void inf_cargar_alumnos(QString letra);
	// Tareas asociadas a la pagina de administracion activa
	void administracion_activa(int actual);
	void pag_activa(int actual);
    // Guardar las configuraciones del programa
    void guardarOpciones();
    void cargarOpciones();
};

#endif // CONOTAS_H
