// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: procedimientoscomandos.js
// Descripci�n : 
//		Este fichero implementa las funciones javascript del fichero procedimientoscomandos.php
// *************************************************************************************************************************************************
var currentidprocedimientocomando=null;
//________________________________________________________________________________________________________
function gestion_comandos(id,o,orden){
	if (o.checked)
		var wurl="../gestores/gestor_procedimientoscomandos.php?opcion="+op_modificacion+"&idprocedimientocomando="+id+"&orden="+orden
	else{
		var resul=window.confirm(TbMsg[0]);
		if (!resul){
			o.checked=true;
			return;
		}
		var wurl="../gestores/gestor_procedimientoscomandos.php?opcion="+op_eliminacion+"&idprocedimientocomando="+id
	}
	currentidprocedimientocomando=id // Guarda identificdor de la procedimiento
	ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
	ifr.src=wurl; // LLama a la p�gina gestora
}
//________________________________________________________________________________________________________
function ActualizarAccion(id){
	var objorden=document.getElementById("orden-"+id)
	var orden=objorden.value
	if(orden=="" || orden<1){
			alert(TbMsg[1]);
			oorden.focus();
			return
	}
	var ocheckbox=document.getElementById("checkbox-"+id);
	gestion_comandos(id,ocheckbox,orden)
}
//________________________________________________________________________________________________________
function resultado_eliminar_procedimientocomando(resul,descrierror,idtt){
	if (!resul){ // Ha habido alg�n error en la eliminaci�n
		alert(descrierror)
		return
	}
	alert(TbMsg[2])
	var oTABLE=document.getElementById("TABLACOMANDOS");
	var oTRs=oTABLE.getElementsByTagName('TR')
	for(var i=0;i<oTRs.length;i++){
			if(oTRs[i].getAttribute("id")=='TR-'+currentidprocedimientocomando || oTRs[i].getAttribute("id")=='PAR-'+currentidprocedimientocomando)
					oTRs[i].style.display="none"
	}
}
//________________________________________________________________________________________________________
function resultado_modificar_procedimientocomando(resul,descrierror,id){
	if (!resul){ // Ha habido alg�n error en la eliminaci�n
		alert(descrierror)
		return
	}
	alert(TbMsg[3])
}
//________________________________________________________________________________________________________
function ver_parametros(o,sw,ida){
		o=o.parentNode
		o.childNodes[sw].style.display="none"
		sw++
		if(sw>1)sw=0
		o.childNodes[sw].style.display="block"
		while (o.tagName!="TBODY"){
			o=o.parentNode
		}
		var oTRs=o.getElementsByTagName('TR')
		for(var i=0;i<oTRs.length;i++){
			if(oTRs[i].getAttribute("id")=='NOT_'+ida || oTRs[i].getAttribute("id")=='PAR-'+ida)
				if (oTRs[i].style.display=="none") oTRs[i].style.display="block"
				else
					oTRs[i].style.display="none"
		}
	}
