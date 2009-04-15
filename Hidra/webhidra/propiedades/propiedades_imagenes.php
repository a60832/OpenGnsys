<? 
// *************************************************************************************************************************************************
// Aplicaci�n WEB: Hidra
// Copyright 2003-2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n: A�o 2003-2004
// Fecha �ltima modificaci�n: MArzo-2005
// Nombre del fichero: propiedades_imagenes.php
// Descripci�n : 
//		 Presenta el formulario de captura de datos de una imagen para insertar,modificar y eliminar
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../includes/opciones.php");
include_once("../includes/CreaComando.php");
include_once("../clases/AdoPhp.php");
include_once("../includes/TomaDato.php");
include_once("../includes/HTMLSELECT.php");
include_once("../idiomas/php/".$idioma."/propiedades_imagenes_".$idioma.".php");
//________________________________________________________________________________________________________
$opcion=0;
$opciones=array($TbMsg[0],$TbMsg[1],$TbMsg[2],$TbMsg[3]);
//________________________________________________________________________________________________________
$idimagen=0; 
$descripcion="";
$idperfilsoft=0;
$comentarios="";
$grupoid=0;

if (isset($_GET["opcion"])) $opcion=$_GET["opcion"];  // Recoge parametros
if (isset($_GET["idimagen"])) $idimagen=$_GET["idimagen"]; 
if (isset($_GET["grupoid"])) $grupoid=$_GET["grupoid"]; 
if (isset($_GET["identificador"])) $idimagen=$_GET["identificador"];
//________________________________________________________________________________________________________
$cmd=CreaComando($cadenaconexion); // Crea objeto comando
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2'); // Error de conexi�n con servidor B.D.
if  ($opcion!=$op_alta){
	$resul=TomaPropiedades($cmd,$idimagen);
	if (!$resul)
		Header('Location: '.$pagerror.'?herror=3'); // Error de recuperaci�n de datos.
}
//________________________________________________________________________________________________________
?>
<HTML>
<TITLE>Administraci�n web de aulas</TITLE>
<HEAD>
	<LINK rel="stylesheet" type="text/css" href="../hidra.css">
	<SCRIPT language="javascript" src="../jscripts/propiedades_imagenes.js"></SCRIPT>
	<SCRIPT language="javascript" src="../jscripts/opciones.js"></SCRIPT>
	<? echo '<SCRIPT language="javascript" src="../idiomas/javascripts/'.$idioma.'/propiedades_imagenes_'.$idioma.'.js"></SCRIPT>'?>
</HEAD>
<BODY>
<DIV  align=center>
<FORM  name="fdatos"> 
	<INPUT type=hidden name=idimagen value=<?=$idimagen?>>
	<INPUT type=hidden name=grupoid value=<?=$grupoid?>>
	<P align=center class=cabeceras><?echo $TbMsg[4]?><BR>
	<SPAN align=center class=subcabeceras><? echo $opciones[$opcion]?></SPAN></P>
	<TABLE  align=center border=0 cellPadding=1 cellSpacing=1 class=tabla_datos>
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
	<TR>
			<TH align=center>&nbsp;<?echo $TbMsg[5]?>&nbsp;</TD>
			<?if ($opcion==$op_eliminacion)
					echo '<TD style="width:300">'.$descripcion.'</TD>';
				else
					echo '<TD><INPUT  class="formulariodatos" name=descripcion style="width:300" type=text value="'.$descripcion.'"></TD>';?>
		</TR>
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
		<TR>
			<TH align=center>&nbsp;<?echo $TbMsg[6]?>&nbsp;</TD>
			<?
				if ($opcion==$op_eliminacion)
					echo '<TD>'.TomaDato($cmd,$idcentro,'perfilessoft',$idperfilsoft,'idperfilsoft','descripcion').'</TD>';
				else
					echo '<TD>'.HTMLSELECT($cmd,$idcentro,'perfilessoft',$idperfilsoft,'idperfilsoft','descripcion',300).'</TD>';
			?>
		</TR>
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
	<TR>
			<TH align=center>&nbsp;<?echo $TbMsg[7]?>&nbsp;</TD>
			<?if ($opcion==$op_eliminacion)
					echo '<TD>'.$comentarios.'</TD>';
				else
					echo '<TD><TEXTAREA   class="formulariodatos" name=comentarios rows=3 cols=55>'.$comentarios.'</TEXTAREA></TD>';
			?>
		</TR>	
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
	</TABLE>
</FORM>
<?
//________________________________________________________________________________________________________
include_once("../includes/opcionesbotonesop.php");
//________________________________________________________________________________________________________
//________________________________________________________________________________________________________
include_once("../includes/iframecomun.php");
//________________________________________________________________________________________________________
?>
</BODY>
</HTML>
<?
//________________________________________________________________________________________________________
//	Recupera los datos de una imagen
//		Parametros: 
//		- cmd: Una comando ya operativo (con conexi�n abierta)  
//		- id: El identificador de la imagen
//________________________________________________________________________________________________________
function TomaPropiedades($cmd,$id){
	global $descripcion;
	global $comentarios;
	global $idperfilsoft;
	$rs=new Recordset; 
	$cmd->texto="SELECT * FROM imagenes WHERE idimagen=".$id;
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return(0); // Error al abrir recordset
	$rs->Primero(); 
	if (!$rs->EOF){
		$descripcion=$rs->campos["descripcion"];
		$idperfilsoft=$rs->campos["idperfilsoft"];
		$comentarios=$rs->campos["comentarios"];
		$rs->Cerrar();
		return(true);
	}
	else
		return(false);
}
?>