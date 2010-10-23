<?
//// ******************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n: A�o 2009-2010
// Fecha �ltima modificaci�n: Agosto-2010
// Nombre del fichero: inclusionacciones.php
// Descripci�n : 
//		Permite incorporar procedimientos y comandos a tareas y tareas ya existentes
// *******************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../includes/CreaComando.php");
include_once("../includes/constantes.php");
include_once("../includes/comunes.php");
include_once("../includes/TomaDato.php");	
include_once("../idiomas/php/".$idioma."/inclusionacciones_".$idioma.".php");
//________________________________________________________________________________________________________

$idtipoaccion=0; 
$tipoaccion=0; 
$descripcionaccion=""; 
$ambito=""; 

if (isset($_GET["idtipoaccion"])) $idtipoaccion=$_GET["idtipoaccion"]; 
if (isset($_GET["tipoaccion"])) $tipoaccion=$_GET["tipoaccion"]; 
if (isset($_GET["descripcionaccion"])) $descripcionaccion=$_GET["descripcionaccion"]; 
if (isset($_GET["ambito"])) $ambito=$_GET["ambito"]; 

//________________________________________________________________________________________________________

$cmd=CreaComando($cadenaconexion); // Crea objeto comando
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2'); // Error de conexi�n con servidor B.D.

$tbParametros=CreaTablaParametros($cmd); // Crea tabla en memmoria para acceder a detalles de comandos 	
//________________________________________________________________________________________________________
?>
<HTML>
	<HEAD>
		<META http-equiv="Content-Type" content="text/html;charset=UTF-8">
		<LINK rel="stylesheet" type="text/css" href="../estilos.css">
		<SCRIPT language="javascript" src="../clases/jscripts/MenuContextual.js"></SCRIPT>		
		<SCRIPT language="javascript" src="../jscripts/inclusionacciones.js"></SCRIPT>
		<SCRIPT language="javascript" src="../clases/jscripts/HttpLib.js"></SCRIPT>	
		<SCRIPT language="javascript" src="../jscripts/comunes.js"></SCRIPT>			
		<SCRIPT language="javascript" src="../idiomas/javascripts/<? echo $idioma?>/inclusionacciones_<? echo $idioma?>.js"></SCRIPT>
	</HEAD>
	<BODY>
		<?
			switch($tipoaccion){
				case $AMBITO_PROCEDIMIENTOS:
					$urlimg="../images/iconos/procedimiento.gif";
					$literal=$TbMsg[0];
					$litcab=$TbMsg[2];
					break;							
				case $AMBITO_TAREAS:
					$urlimg="../images/iconos/tareas.gif";
					$literal=$TbMsg[1];	
					$litcab=$TbMsg[3];					
					break;					
			}	
		?>
		<P align=center class=cabeceras><IMG src="../images/iconos/acciones.gif">&nbsp;<?echo $literal?><BR>		
		<SPAN align=center class=subcabeceras><?echo $descripcionaccion?></SPAN>&nbsp;<IMG src="<? echo $urlimg?>"></P>
	
		<TABLE align=center border=0>
			<TR>
		<?
			$conTR=0; // Contador de lineas (Identificadores de las filas)
			switch($tipoaccion){
				case $AMBITO_PROCEDIMIENTOS:
					$litmsg=$TbMsg[5];				
					listaProcedientos($cmd,$idtipoaccion);
					break;							
				case $AMBITO_TAREAS:
					$litmsg=$TbMsg[6];				
					listaTareas($cmd,$idtipoaccion,$ambito);
					break;					
			}	
		?>	
			</TR>
		</TABLE>
		<FORM name="fdatos">
			<INPUT type="hidden" name="conTR" value="<?echo $conTR?>">
			<INPUT type="hidden" name="tipoaccion" value="<?echo $tipoaccion?>">
			<INPUT type="hidden" name="idtipoaccion" value="<?echo $idtipoaccion?>">
		</FORM>
	</BODY>
</HTML>
<?
// *************************************************************************************************************************************************
	function listaProcedientos($cmd,$idprocedimiento)
	{
		global $AMBITO_PROCEDIMIENTOS;
		global $TbMsg;	
		
		$cmd->texto="SELECT procedimientos_acciones.idprocedimientoaccion as identificador,procedimientos_acciones.orden,
				procedimientos_acciones.parametros,procedimientos_acciones.procedimientoid,
				comandos.idcomando,comandos.descripcion as comando,
				procedimientostemp.descripcion as procedimiento
				FROM procedimientos
				INNER JOIN procedimientos_acciones ON procedimientos_acciones.idprocedimiento=procedimientos.idprocedimiento
				LEFT OUTER JOIN comandos ON comandos.idcomando=procedimientos_acciones.idcomando
				LEFT OUTER JOIN procedimientos as procedimientostemp 
							ON procedimientostemp.idprocedimiento=procedimientos_acciones.procedimientoid 
				WHERE procedimientos.idprocedimiento=".$idprocedimiento." 
				ORDER by procedimientos_acciones.orden";
				
		//echo $cmd->texto;
	
		$idprocedimientos=escribeAcciones($cmd,$AMBITO_PROCEDIMIENTOS,true,7);
		
		//$idprocedimientos.=$idprocedimiento; // Excluye a �l mismo de la segunda ronda
		$idprocedimientos=$idprocedimiento; // Excluye  s�lo a �l para evitar dead-lock
		$cmd->texto="SELECT idprocedimiento as identificador, 0 as orden, 0 as idcomando,descripcion as procedimiento
					FROM procedimientos
					WHERE idprocedimiento NOT IN (".$idprocedimientos.") 
					ORDER BY descripcion";
				
		escribeAcciones($cmd,$AMBITO_PROCEDIMIENTOS,false,8);
	}
// *************************************************************************************************************************************************
	function listaTareas($cmd,$idtarea,$ambito)
	{
		global $AMBITO_PROCEDIMIENTOS;	
		global $AMBITO_TAREAS;
		global $AMBITO_TAREAS;
		global $TbMsg;	
		
		/* Contenido de tarea */ 
		$cmd->texto="SELECT tareas.ambito,tareas.idambito,tareas_acciones.idtareaaccion as identificador,tareas_acciones.orden,
				tareas_acciones.tareaid,procedimientos.idprocedimiento,procedimientos.descripcion as procedimiento,
				tareastemp.descripcion as tarea
				FROM tareas
				INNER JOIN tareas_acciones ON tareas_acciones.idtarea=tareas.idtarea
				LEFT OUTER JOIN procedimientos ON procedimientos.idprocedimiento=tareas_acciones.idprocedimiento
				LEFT OUTER JOIN tareas as tareastemp 
							ON tareastemp.idtarea=tareas_acciones.tareaid 
				WHERE tareas.idtarea=".$idtarea." 
				ORDER by tareas_acciones.orden";
				
		//echo $cmd->texto;
		$idtareas=escribeAcciones($cmd,$AMBITO_TAREAS,true,7);
		
		/* Procedimientos disponibles */
		if(!empty($ambito)){
			$cmd->texto="SELECT idprocedimiento as identificador, 0 as orden, 0 as idcomando,descripcion as procedimiento
						FROM procedimientos
						ORDER BY descripcion";	

			escribeAcciones($cmd,$AMBITO_PROCEDIMIENTOS,false,8);	
		}
		
		/* Tareas disponibles */ 
		//$idtareas.=$idtarea; // Excluye a ella misma de la segunda ronda
		$idtareas=$idtarea; // Excluye  s�lo a ella para evitar dead-lock
		$cmd->texto="SELECT idtarea as identificador, 0 as orden, 0 as idprocedimiento,	descripcion as tarea
					FROM tareas
					WHERE idtarea NOT IN (".$idtareas.") 
					ORDER BY descripcion";
		escribeAcciones($cmd,$AMBITO_TAREAS,false,9);
	}	
//________________________________________________________________________________________________________

	function escribeAcciones($cmd,$tipoaccion,$sw,$imsg)
	{
		global $AMBITO_PROCEDIMIENTOS;
		global $AMBITO_TAREAS;
		global $AMBITO_COMANDOS;
		global $conTR;
		global $TbMsg;
		
		$rs=new Recordset; 
		$rs->Comando=&$cmd; 		
		if (!$rs->Abrir()) return("");
	
		$idacciones="";
		
		echo '<TD valign=top>';
		if($rs->EOF && !sw){
			echo '</P></TD>';
			return;
		}

		echo '<P align=center><SPAN class=presentaciones><B>'.$TbMsg[$imsg].'</B></SPAN></BR>';		
		echo '	<TABLE align=center class="tabla_listados" cellspacing=1 cellpadding=0>';
		escribeCabecera();	
		while (!$rs->EOF){
			echo '<TR id="TR-'.$conTR.'" value="'.$rs->campos["identificador"].'">';
			$conTR++;
			echo '<TD align=center><INPUT type=checkbox';
			if($sw)
				echo ' value="1" checked '; // Lo marca como seleccionado (1�. ronda)
			else
				echo ' value="0"';
			echo '></TD>';
			echo '<TD align=center ><INPUT class="formulariodatos" style="WIDTH:30px" 
					type="text" value="'.$rs->campos["orden"].'" id="'.$rs->campos["orden"].'"></TD>';

			// Descripcion de la acci�n
			switch($tipoaccion){
				case $AMBITO_PROCEDIMIENTOS:
					if(!empty($rs->campos["idcomando"])){
						$urlimg="../images/iconos/comandos.gif";
						$accion=$rs->campos["comando"];
						$value=$AMBITO_COMANDOS;
					}
					else{
						$urlimg="../images/iconos/procedimiento.gif";
						$accion=$rs->campos["procedimiento"];
						$idacciones.=$rs->campos["procedimientoid"].",";
						$value=$AMBITO_PROCEDIMIENTOS;
					}
					break;							
				case $AMBITO_TAREAS:
					if(!empty($rs->campos["idprocedimiento"])){
						$urlimg="../images/iconos/procedimiento.gif";
						$accion=$rs->campos["procedimiento"];
						$value=$AMBITO_PROCEDIMIENTOS;
					}
					else{
						$urlimg="../images/iconos/tareas.gif";
						$accion=$rs->campos["tarea"];
						$idacciones.=$rs->campos["tareaid"].",";
						$value=$AMBITO_TAREAS;						
					}
					break;					
			}
			echo '<TD align=center><IMG style="cursor:pointer" src="'.$urlimg.'" value="'.$value.'"></TD>';
			echo '<TD>'.$accion.'</TD>';
			// Orden del item del item
			echo '</TR>';
			
			/* Muestra par�metros de los comandos (Se hace en Informaci�n)
				if($tipoaccion==$AMBITO_PROCEDIMIENTOS){
				if(!empty($rs->campos["idcomando"])){ // Se trata de un comando, se muestran par�metros)
					$htmlprm=escribeParametros($rs->campos["parametros"]);
					if(!empty($htmlprm)){
						echo '<TR>';
						echo '<TD>&nbsp;</TD>';
						echo '<TD colspan=3>'.escribeParametros($rs->campos["parametros"]).'</TD>';
					}
				}	
			}
			*/
			$rs->Siguiente();
		}
		echo '</TABLE>';
		if($sw) escribePie();
		echo '</P></TD>';		
		$rs->Cerrar();	
		$idacciones.="0"; // Para evitar el �ltimo ";"		
		return($idacciones);
	}
//________________________________________________________________________________________________________

	function escribeCabecera()
	{	
		global $TbMsg;
		global $litcab;
		
		echo '<TR height=20>
				<TH>&nbsp;</TH>
				<TH>&nbsp;'.$TbMsg[4].'&nbsp;</TH>				
				<TH>T</TH>				
				<TH>&nbsp;'.$litcab.'</TH>
			</TR>';
	}
//________________________________________________________________________________________________________

	function escribePie()
	{	
		global $litmsg;
		global $op;
		
		echo '<BR><TABLE align=center>
				<TR>
					<TD><A href="#botones"><IMG border=0 src="../images/boton_confirmar.gif" 
					onclick="javascript:confirmar('.$op.')"></A></TD>
				</TR>
			</TABLE>
			<BR>		
			<DIV id="Layer_nota" align=center>
				<SPAN align=center class=notas><I>'.$litmsg.'</I></SPAN>
			</DIV>';
	}	
?>