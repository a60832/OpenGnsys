<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Copyright 2003-2005 Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n: Diciembre-2003
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: acceso.php
// Descripci�n : Presenta la pantalla de login de la aplicaci�n
// *************************************************************************************************************************************************
if(isset($_SESSION)){ 	// Si existe algua sesi�n ...
	session_unset(); // Elimina variables
	session_destroy(); // Destruye sesi�n
}
$herror=0;
if (isset($_GET["herror"])) $herror=$_GET["herror"]; 

$TbErr=array();
$TbErr[0]="NO ERRORS";
$TbErr[1]="WARNING:You must access to aplication through login pag";
$TbErr[2]="WARNING:Aplication without access to Server Data Base";
$TbErr[3]="WARNING: There are some problem to recovery the record, must be it is removed";
//________________________________________________________________________________________________________
?>
<HTML>
<TITLE> Administraci�n web de aulas</TITLE>
<HEAD>
	<LINK rel="stylesheet" type="text/css" href="estilos.css">
</HEAD>
<SCRIPT LANGUAGE="JAVASCRIPT">
//________________________________________________________________________________________________________
function confirmar(){
	if (comprobar_datos()){
		var usu=document.fdatos.usu.value;
		var pss=document.fdatos.pss.value;
		var ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
		var wurl="controlacceso.php?usu="+usu+"&pss="+pss
		ifr.src=wurl; // LLama a la p�gina gestora
	}
}
//________________________________________________________________________________________________________
function comprobar_datos(){
	if (document.fdatos.usu.value==""){
		alert("You must introduce a User name")
		document.fdatos.usu.focus()
		return(false)
	}
	if (document.fdatos.pss.value==""){
		alert("You must introduce a Password")
		document.fdatos.pss.focus()
		return(false)
	}
	return(true)
}
//______________________________________________________________________________________________________
function resultado_acceso(resul){
	if (!resul){
		alert('WARNING: You are not authorized to access this aplication')
		return
	}
	location.href="frames.php";
}
//______________________________________________________________________________________________________
function PulsaEnter(oEvento){ 
    var iAscii; 
    if (oEvento.keyCode) 
        iAscii = oEvento.keyCode; 
    else{
		if (oEvento.which) 
			iAscii = oEvento.which; 
		else 
			return false; 
	}
    if (iAscii == 13)  confirmar();
	return true; 
} 
//________________________________________________________________________________________________________
</SCRIPT>
</HEAD>
<BODY>
<DIV style="POSITION:absolute;top:90;left:212">
	<FORM action="controlacceso.php" name="fdatos" method="post">
		<DIV align="center">
			<IMG src="./images/login_eng.jpg" >
			<INPUT onkeypress="PulsaEnter(event)" name="usu"  style="POSITION:absolute;top:160;left:455;width=130;height:20;COLOR: #999999; FONT-FAMILY: Verdana; FONT-SIZE: 12px;">
			<INPUT onkeypress="PulsaEnter(event)"  name="pss" type="password"  style="POSITION:absolute;top:190;left:455;width=130;height:20;COLOR: #999999; FONT-FAMILY: Verdana; FONT-SIZE: 12px;">
			<IMG onclick="confirmar()" src="./images/botonok.gif" style="POSITION:absolute;top:215;left:555;CURSOR: hand">
		</DIV>
	</FORM>
</DIV>
<?
//________________________________________________________________________________________________________
echo '<DIV  style="POSITION: absolute;LEFT: 20px;TOP:300px;visibility=hidden" height=300 width=300>';
echo '<IFRAME scrolling=yes height=300 width=310 id="iframes_comodin" src="./nada.php"></IFRAME>';
echo '</DIV>';
//________________________________________________________________________________________________________
// Posiciona cursor en campo usuario y muestra mensaje de error si lo hubiera
echo '<SCRIPT LANGUAGE="javascript">';
if (!empty($herror))
	echo "	alert('".$TbErr[$herror]."');";
echo 'document.fdatos.usu.focus()';
echo '</SCRIPT>';
//________________________________________________________________________________________________________
?>
</BODY>
</HTML>
