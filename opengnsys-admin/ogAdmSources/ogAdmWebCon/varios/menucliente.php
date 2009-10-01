<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Copyright 200-2005 Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n: A�o 2003-2005
// Fecha �ltima modificaci�n: Abril-2005
// Nombre del fichero: menumliente.php
// Descripci�n :
//		Pagina del menu del cliente. �ste la solicita a trav�s de su browser local
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../includes/constantes.php");
include_once("../includes/CreaComando.php");
//________________________________________________________________________________________________________
$ipordenador="0.0.0.0";

if (isset($_GET["ip"]))	$ipordenador=$_GET["ip"]; 
//________________________________________________________________________________________________________
$cmd=CreaComando($cadenaconexion);
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2');  // Error de conexi�n con servidor B.D.
//________________________________________________________________________________________________________
$rsmenu=RecuperaMenu($cmd,$ipordenador);	// Recupera un recordset con los datos del m en�
?>
	<HTML>
	<HEAD>
	</HEAD>
	<BODY>
<?	
if(!empty($rsmenu)){
	$codeHtml=GeneraMenu($rsmenu,1); // Genera men� p�blico
	echo $codeHtml;
}
else
	echo '<H1>NO SE HA DETCTADO NING�N MEN� PARA ESTE CLIENTE</H1>';	
?>
	</BODY>
	</HTML>
<?
//___________________________________________________________________________________________________
//
// Recupera Men�
//___________________________________________________________________________________________________
function RecuperaMenu($cmd,$ipordenador){
	$rs=new Recordset; 
	$cmd->texto="SELECT menus.resolucion,menus.titulo,menus.coorx,menus.coory,menus.modalidad,
						menus.scoorx,menus.scoory,menus.smodalidad,menus.htmlmenupub,menus.htmlmenupri,
						acciones_menus.tipoaccion,acciones_menus.idaccionmenu,acciones_menus.idtipoaccion,
						acciones_menus.tipoitem,acciones_menus.descripitem,acciones_menus.idurlimg 
						FROM ordenadores
						INNER JOIN menus ON menus.idmenu = ordenadores.idmenu 
						INNER JOIN acciones_menus ON acciones_menus.idmenu = menus.idmenu
						WHERE ordenadores.ip='".$ipordenador."' ORDER by acciones_menus.orden";
	$rs->Comando=&$cmd; 
	$resul=$rs->Abrir();
	if (!$rs->Abrir()) return(false);
	if ($rs->EOF) return(false);
	return($rs);
}
//___________________________________________________________________________________________________
//
// Muestra el menu p�blico
//___________________________________________________________________________________________________
function GeneraMenu($rs,$tipo){	
	$titulo=$rs->campos["titulo"]; 
	$coorx=$rs->campos["coorx"]; 
	$coory=$rs->campos["coory"]; 
	$modalidad=$rs->campos["modalidad"]; 
	$scoorx=$rs->campos["scoorx"]; 
	$scoory=$rs->campos["scoory"]; 
	$smodalidad=$rs->campos["smodalidad"]; 
	$scoory=$rs->campos["scoory"]; 
	$resolucion=$rs->campos["resolucion"]; 
	$htmlmenupub=$rs->campos["htmlmenupub"]; 
	$htmlmenupri=$rs->campos["htmlmenupri"]; 

	//	Genera HTML de la p�gina en funci�n de las propiedades del Men� del clioente
	$codeHTML='<DIV style="POSITION:absolute;TOP:'.$coory.";LEFT:".$coorx.'">';
	$codeHTML.='<H1>'.$titulo.'</H1>';	
	$codeHTML.='<TABLE align="center" style="font-family:sans-serif;color: #a71026">';
	$codeHTML.='<TR>';

	$c=0; // Contador de columnas
	
	while (!$rs->EOF){ // Recorre acciones del menu
		$tipoitem=$rs->campos["tipoitem"]; 
		if($tipoitem==$tipo){
			$tipoaccion=$rs->campos["tipoaccion"]; 
			$idtipoaccion=$rs->campos["idtipoaccion"]; 
			$idaccionmenu=$rs->campos["idaccionmenu"]; 
			$descripitem=$rs->campos["descripitem"]; 
			$idurlimg=$rs->campos["idurlimg"]; 
			$codeHTML.='<TD><IMG src="../images/iconos/confirmadas.gif"></TD>';
			$codeHTML.='<TD>'.$descripitem.'</TD>';
			if($c%$modalidad==0){
				$codeHTML.='</TR>';
				$codeHTML.='<TR>';
			}
			$codeHTML.='</TR>';
		}
		$rs->Siguiente();
	}
	$rs->Cerrar();
	return($codeHTML);
}
?>
