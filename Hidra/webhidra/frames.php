<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: Hidra
// Copyright 2003-2005 Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n: Diciembre-2003
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: frames.php
// Descripci�n :Este fichero implementa la distribuci�n en frames de la aplicaci�n
// *************************************************************************************************************************************************
include_once("./includes/ctrlacc.php");
include_once("./includes/constantes.php");

//________________________________________________________________________________________________________
?>
<HTML>
<HEAD>
<TITLE> Administraci�n web de aulas</TITLE>
</HEAD>
<FRAMESET rows="25,*">
	<FRAME SRC="barramenu.php" frameborder=0  scrolling=no  NAME="frame_menus" >
	<FRAMESET cols="30%,*">
			<? if($idtipousuario!=$SUPERADMINISTRADOR){?>
					<FRAME SRC="./principal/aulas.php" frameborder=1 scrolling=auto NAME="frame_arbol" >
				<? }
						else{
									if($idtipousuario==$SUPERADMINISTRADOR){?>
										<FRAME SRC="./principal/administracion.php" frameborder=1 scrolling=auto NAME="frame_arbol" >
									<?}?>
						<?}?>
		<FRAME SRC="nada.php" frameborder=1  NAME="frame_contenidos">
		</FRAMESET>
	</FRAMESET>	
</FRAMESET>
</HTML>
