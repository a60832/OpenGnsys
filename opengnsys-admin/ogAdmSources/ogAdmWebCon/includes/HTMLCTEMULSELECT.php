<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon.
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: HTMLCTEMULSELECT.php
// Descripci�n :
//		Crea la etiqueta html <SELECT> multiselecci�n, de valores constantes
//	Parametros: 
//		- parametros: Una cadena con la forma valor=literal separada por un caracter INTRO
//		- nombreid: Nombre del desplegable (atributo HTML name)
//		- tbvalor: Array con los valores de las opciones que aparecer�n seleccionadas
//		- clase: Clase que define su estilo
//		- eventochg: Nombre de la funci�n que se ejecutar� en respuesta al evento onchange( por defecto: ninguna)
//		- ancho: Anchura del desplegable
//		- alto: Altura del desplegable
// *************************************************************************************************************************************************
function HTMLCTEMULSELECT($parametros,$nombreid,$tbvalor,$clase,$eventochg,$ancho,$alto){
	if (!empty($eventochg)) $eventochg='onchange="'.$eventochg.'(this);"';
	if (empty($clase))	$clase='formulariodatos';
	$x=0;
	$opciones=split(chr(13),$parametros);
	$SelectHtml= '<SELECT '.$eventochg.' class="'.$clase.'" name="'.$nombreid.'" multiple size='.$alto.' style="WIDTH: '.$ancho.'">';
	for($i=0;$i<sizeof($opciones);$i++){
		$item=split("=",$opciones[$i]);
		$SelectHtml.= '<OPTION value="'.$item[0].'"';
		if (isset($tbvalor[$x])){
			if($tbvalor[$x]==$item[0]) {
				$SelectHtml.=" selected ";
				$x++;
			}
		}
		$SelectHtml.= '>'.$item[1].'</OPTION>';
	}
	return($SelectHtml);
}