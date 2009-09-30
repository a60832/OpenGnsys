<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n: A�o 2003-2004
// Fecha �ltima modificaci�n: Marzo-2005
// Nombre del fichero: gestor_softincrementales.php
// Descripci�n :
//		Gestiona el mantenimiento de la tabla de softincrementales
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../clases/XmlPhp.php");
include_once("../clases/ArbolVistaXML.php");
include_once("../includes/CreaComando.php");
include_once("../includes/constantes.php");
include_once("../includes/opciones.php");
include_once("./relaciones/incrementales_eliminacion.php");
//________________________________________________________________________________________________________
$opcion=0; // Inicializa parametros

$idsoftincremental=0; 
$descripcion="";
$comentarios="";
$grupoid=0; 

if (isset($_GET["opcion"])) $opcion=$_GET["opcion"]; // Recoge parametros

if (isset($_GET["idsoftincremental"])) $idsoftincremental=$_GET["idsoftincremental"];
if (isset($_GET["descripcion"])) $descripcion=$_GET["descripcion"]; 
if (isset($_GET["comentarios"])) $comentarios=$_GET["comentarios"]; 
if (isset($_GET["grupoid"])) $grupoid=$_GET["grupoid"];
if (isset($_GET["identificador"])) $idsoftincremental=$_GET["identificador"];

$tablanodo=""; // Arbol para nodos insertados

$cmd=CreaComando($cadenaconexion); // Crea objeto comando
$resul=false;
if ($cmd){
	$resul=Gestiona();
	$cmd->Conexion->Cerrar();
}
// *************************************************************************************************************************************************
?>
<HTML>
<HEAD>
<BODY>
<?
	$literal="";
	switch($opcion){
		case $op_alta :
			$literal="resultado_insertar_softincrementales";
			break;
		case $op_modificacion:
			$literal="resultado_modificar_softincrementales";
			break;
		case $op_eliminacion :
			$literal="resultado_eliminar_softincrementales";
			break;
		case $op_movida :
			$literal="resultado_mover";
			break;	
		default:
			break;
	}
echo '<p><span id="arbol_nodo">'.$tablanodo.'</span></p>';
if ($resul){
	echo '<SCRIPT language="javascript">'.chr(13);
	echo 'var oHTML'.chr(13);
	echo 'var cTBODY=document.getElementsByTagName("TBODY");'.chr(13);
	echo 'o=cTBODY.item(1);'.chr(13);
	if ($opcion==$op_alta )
		echo 'window.parent.'.$literal."(1,'".$cmd->DescripUltimoError()." ',".$idsoftincremental.",o.innerHTML);";
	else
		echo 'window.parent.'.$literal."(1,'".$cmd->DescripUltimoError()." ','".$descripcion."');";
	echo '</SCRIPT>';
}
else{
	echo '<SCRIPT language="javascript">';
	echo "	window.parent.".$literal."(0,'".$cmd->DescripUltimoError()."',".$idsoftincremental.")";
	echo '</SCRIPT>';
}
?>
</BODY>
</HTML>	
<?
/**************************************************************************************************************************************************
	Inserta, modifica o elimina datos en la tabla softincrementales
________________________________________________________________________________________________________*/
function Gestiona(){
	global	$cmd;
	global	$opcion;

	global	$idcentro;
	global	$idsoftincremental;
	global	$descripcion;
	global	$comentarios;
	global	$grupoid;

	global	$op_alta;
	global	$op_modificacion;
	global	$op_eliminacion;
	global	$op_movida;

	global	$tablanodo;

	$cmd->CreaParametro("@idcentro",$idcentro,1);
	$cmd->CreaParametro("@idsoftincremental",$idsoftincremental,1);
	$cmd->CreaParametro("@descripcion",$descripcion,0);
	$cmd->CreaParametro("@comentarios",$comentarios,0);
	$cmd->CreaParametro("@grupoid",$grupoid,1);

	switch($opcion){
		case $op_alta :
			$cmd->texto="INSERT INTO softincrementales (descripcion,comentarios,idcentro,grupoid) VALUES (@descripcion,@comentarios,@idcentro,@grupoid)";
			$resul=$cmd->Ejecutar();
			if ($resul){ // Crea una tabla nodo para devolver a la p�gina que llam� �sta
				$idsoftincremental=$cmd->Autonumerico();
				$arbolXML=SubarbolXML_softincrementales($idsoftincremental,$descripcion);
				$baseurlimg="../images/signos"; // Url de las imagenes de signo
				$clasedefault="texto_arbol"; // Hoja de estilo (Clase por defecto) del �rbol
				$arbol=new ArbolVistaXML($arbolXML,0,$baseurlimg,$clasedefault);
				$tablanodo=$arbol->CreaArbolVistaXML();
			}
			break;
		case $op_modificacion:
			$cmd->texto="UPDATE softincrementales SET descripcion=@descripcion,comentarios=@comentarios WHERE idsoftincremental=@idsoftincremental";
			$resul=$cmd->Ejecutar();
			break;
		case $op_eliminacion :
			$resul=EliminaSoftincremental($cmd,$idsoftincremental,"idsoftincremental");
			break;
		case $op_movida :
			$cmd->texto="UPDATE softincrementales SET  grupoid=@grupoid WHERE idsoftincremental=@idsoftincremental";
			$resul=$cmd->Ejecutar();
			break;
		default:
			break;
	}
	return($resul);
}
/*________________________________________________________________________________________________________
	Crea un arbol XML para el nuevo nodo insertado 
________________________________________________________________________________________________________*/
function SubarbolXML_softincrementales($idsoftincremental,$descripcion){
		global $LITAMBITO_SOFTINCREMENTAL;
		$cadenaXML='<SOFTINCREMENTAL ';
		// Atributos		
		$cadenaXML.=' imagenodo="../images/iconos/incremental.gif"';
		$cadenaXML.=' infonodo="' .$descripcion.'"';
		$cadenaXML.=' nodoid='.$LITAMBITO_SOFTINCREMENTAL.'-'.$idsoftincremental;
		$cadenaXML.=' clickcontextualnodo="menu_contextual(this,' ."'flo_".$LITAMBITO_SOFTINCREMENTAL."'" .')"';
		$cadenaXML.='>';
		$cadenaXML.='</SOFTINCREMENTAL>';
		return($cadenaXML);

}
?>