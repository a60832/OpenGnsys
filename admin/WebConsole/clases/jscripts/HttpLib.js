// ************************************************************************************************//
// Aplicaci�n WEB: Realtab
//		Descripci�n: Aplicaci�n web para gesti�n de Tablaturas y acordes de guitarra
// 	Copyright 2009 Jos� Manuel Alonso. Todos los derechos reservados.
// 	Fecha: Junio 2008
//  Fichero: Clase para llamar p�ginas web usando metodolog�a AJAX
// *************************************************************************************************//
var _url;
var _fun;
var oXMLHttpRequest;
//____________________________________________________________________________
//	
//	LLama a la p�gina
//
//	Par�metros:
//	
//		url			// Url de la p�gina a la que se llama
//		fun			// Funci�n a la que se llama despues de descargarse la p�gina
//____________________________________________________________________________
function CallPage(url,fun){
	 _url=url;
	 _fun=fun;
	if (window.XMLHttpRequest) {
		oXMLHttpRequest= new XMLHttpRequest();
		oXMLHttpRequest.onreadystatechange = procesaoXMLHttpRequest;
		oXMLHttpRequest.open("GET",_url, true);
		oXMLHttpRequest.send(null);    
	} else if (window.ActiveXObject) {
		isIE = true;
		try {
		  oXMLHttpRequest= new ActiveXObject("Msxml2.XMLHTTP");
		  } catch (e) {
			try {
			  oXMLHttpRequest= new ActiveXObject("Microsoft.XMLHTTP");
			} catch (E) {
			  oXMLHttpRequest= false;
			}
		}
		if (oXMLHttpRequest) {
			oXMLHttpRequest.onreadystatechange =procesaoXMLHttpRequest;
			oXMLHttpRequest.open("GET",_url, true);
			oXMLHttpRequest.send();
		}
	}
}
 //_____________________________________________________________________________________
 function procesaoXMLHttpRequest(){
	if (oXMLHttpRequest.readyState == 4) {
		if (oXMLHttpRequest.status == 200) {
			var fcbk=_fun+"(oXMLHttpRequest.responseText)";
			eval(fcbk)
		 } 
	}
}