<?
/*______________________________________________________________________
	Redirecciona a la p�gina de error
	Parametros: 
		- Literal del error
_______________________________________________________________________*/
function RedireccionaError($herror){

	$urlerror=urlencode($herror);
	$wurl="../seguridad/logerror.php?herror=".$urlerror;
	Header('Location: '.$wurl);
}
?>