<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: Hidra
// Copyright 2003-2005 Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n: Diciembre-2003
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: acceso.php
// Descripci�n : Presenta la pantalla de login de la aplicaci�n
// *************************************************************************************************************************************************
$herror=0;
if (isset($_GET["herror"])) $herror=$_GET["herror"]; 
if (isset($_POST["herror"])) $herror=$_POST["herror"]; 

$TbErr=array();
$TbErr[0]="SIN ERRORES";
$TbErr[1]="ATENCI�N: Debe acceder a la aplicaci�n a trav�s de la pagina inicial";
$TbErr[2]="ATENCI�N: La Aplicaci�n no tiene acceso al Servidor de Bases de Datos";
$TbErr[3]="ATENCI�N: Existen problemas para recuperar el registro, puede que haya sido eliminado";
$TbErr[4]="ATENCI�N: Usted no tiene acceso a esta aplicaci�n";
//________________________________________________________________________________________________________
?>
<HTML>
<TITLE>Administraci�n web de aulas</TITLE>
<HEAD>
	<LINK rel="stylesheet" type="text/css" href="hidra.css">
</HEAD>
<SCRIPT LANGUAGE="JAVASCRIPT">
//________________________________________________________________________________________________________
function confirmar(){
	if (comprobar_datos())
		document.fdatos.submit();
}
//________________________________________________________________________________________________________
function comprobar_datos(){
	if (document.fdatos.usu.value==""){
		alert("Debe introducir un nombre de Usuario")
		document.fdatos.usu.focus()
		return(false)
	}
	if (document.fdatos.pss.value==""){
		alert("Debe introducir una contrase�a")
		document.fdatos.pss.focus()
		return(false)
	}
	return(true)
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
			<IMG src="./images/login_esp.jpg" >
			<INPUT onkeypress="PulsaEnter(event)" name="usu"  style="POSITION:absolute;top:160;left:455;width=130;height:20;COLOR: #999999; FONT-FAMILY: Verdana; FONT-SIZE: 12px;">
			<INPUT onkeypress="PulsaEnter(event)"  name="pss" type="password"  style="POSITION:absolute;top:190;left:455;width=130;height:20;COLOR: #999999; FONT-FAMILY: Verdana; FONT-SIZE: 12px;">
			<IMG onclick="confirmar()" src="./images/botonok.gif" style="POSITION:absolute;top:215;left:555;CURSOR: hand">
		</DIV>
	</FORM>
</DIV>
<?
//________________________________________________________________________________________________________
echo '<DIV  style="POSITION: absolute;LEFT: 20px;TOP:300px;visibility:hidden" height=300 width=300>';
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
