<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon.
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: HTMLCTESELECT.php
// Descripci�n :
//		Crea la etiqueta html <SELECT> de valores constantes
//	Parametros: 
//		- parametros:Una cadena con la forma valor=literal separada por un caracter INTRO
//		- nombreid: Nombre del desplegable (atributo HTML name)
//		- clase: Clase que define su estilo
//		- defaultlit: Literal de la primera opci�n cuyo valor es siempre 0
//		- valorselec: Valor del item que saldr� seleccionado por defecto
//		- ancho: Anchura del desplegable
//		- eventochg: Nombre de la funci�n que se ejecutar� en respuesta al evento onchange
// *************************************************************************************************************************************************
function HTMLCTESELECT($parametros,$nombreid,$clase,$defaultlit,$valorselec,$ancho,$eventochg=""){
	if (!empty($eventochg))	$eventochg='onchange="'.$eventochg.'(this);"';
	$opciones=split(chr(13),$parametros);
	$SelectHtml= '<SELECT '.$eventochg.' class= "'.$clase.'" id='.$nombreid.' name="'.$nombreid.'" style="WIDTH: '.$ancho.'">';
	if (!empty($defaultlit)) $SelectHtml.= '<OPTION value="0">'.$defaultlit.'</OPTION>';
	for($i=0;$i<sizeof($opciones);$i++){
		$item=split("=",$opciones[$i]);
		$SelectHtml.= '<OPTION value="'.$item[0].'"';
		if($valorselec==$item[0]) $SelectHtml.=" selected ";
		$SelectHtml.= '>'.$item[1].'</OPTION>';
	}
	return($SelectHtml);
}