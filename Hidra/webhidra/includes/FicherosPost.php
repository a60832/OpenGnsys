<?
//________________________________________________________________________________________
//
//	Salva un fichero enviado por POST
//	Par�metros:
//		- $UrlPagina: Url de la p�gina que carga el fichero
//		- $NombreFicheroPost: Nombre temporal que le da php al fichero post
//		- $NombreFichero: Nombre definitivo que tendr�  el fichero
//________________________________________________________________________________________
function SalvaFichero_POST($UrlPagina,$NombreFicheroPost,$NombreFichero,$UrlFichero){

	$UrlCarpetaPagina=dirname($UrlPagina);
	$UrlFichero=$UrlCarpetaPagina."/iconos/".$NombreFichero;
	
	$PathFisicoFichero=TomaPathFIsico($UrlPagina,$NombreFichero);

	if (file_exists($PathFisicoFichero)) // Borra el fichero si existe
        unlink($PathFisicoFichero);
	$resul=move_uploaded_file($NombreFicheroPost,$PathFisicoFichero); // salva el fichero
	return($resul);
}
//________________________________________________________________________________________
//
//	Elimina un fichero en el servidor
//	Par�metros:
//		- $UrlPagina: Url de la p�gina que carga el fichero
//		- $NombreFichero: Nombre definitivo que tendr�  el fichero
//________________________________________________________________________________________
function EliminaFichero($UrlPagina,$NombreFichero){
	$PathFisicoFichero=TomaPathFIsico($UrlPagina,$NombreFichero);
	$resul=false;
	if (file_exists($PathFisicoFichero)) // Borra el fichero si existe
       $resul=unlink($PathFisicoFichero);
	return($resul);
}
//________________________________________________________________________________________
//
//	Toma el path f�sico de un fichero
//	Par�metros:
//		- $UrlPagina: Url de la p�gina que carga el fichero
//		- $NombreFichero: Nombre definitivo que tendr�  el fichero
//________________________________________________________________________________________
function TomaPathFisico($UrlPagina,$NombreFichero){
	$Nombrepagina=basename($UrlPagina);
	$PathFisicoPagina=realpath($Nombrepagina);
	$PathFisicoCarpetaPagina=dirname($PathFisicoPagina);
	$PathFisicoCarpetaFichero=$PathFisicoCarpetaPagina."/iconos";
	$PathFisicoFichero=$PathFisicoCarpetaPagina."/iconos/".$NombreFichero;
	return($PathFisicoFichero);
}
?>