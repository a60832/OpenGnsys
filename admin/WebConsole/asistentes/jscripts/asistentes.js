// ***********************************************************************************************************
// Libreria de scripts de Javascript
// Autor: 
// Fecha CreaciÃ³n: 2011
// Fecha Ãltima modificaciÃ³n: enero-2011
// Nombre del fichero: asistentes.js
// DescripciÃ³n : 
//		Este fichero implementa las funciones javascript del fichero AsistentesEjecutarScripts.php (Comandos)
// ***********************************************************************************************************

function codeCloneRemotePartition(form){
switch (form.idmetodo.value)
{
	case "MULTICAST":
 		protocol="MULTICAST " + form.mcastpuerto.value  + ":" + form.mcastmodo.value + ":" + form.mcastdireccion.value + ":" + form.mcastvelocidad.value + "M:" + form.mcastnclien.value + ":" + form.mcastseg.value + " ";
		break;
	case "UNICAST":
		protocol="UNICAST " +  form.ucastport.value + ":" + form.ucastclient.value + " ";
		break;
}
//form.codigo.value="cloneRemoteFromMaster " + form.ipMaster.value + " 1 " + form.PartOrigen.value + "  " + form.mcastpuerto.value  + ":" + form.mcastmodo.value + ":" + form.mcastdireccion.value + ":" + form.mcastvelocidad.value + "M:" + form.mcastnclien.value + ":" + form.mcastseg.value + " 1 " + form.PartOrigen.value + " " + form.tool.value + " " + form.compresor.value;
command="cloneRemoteFromMaster " + form.ipMaster.value + " " + form.source.value + "  " + protocol  + " " + form.targetpart.value + " " + form.tool.value + " " + form.compresor.value;
form.codigo.value="\
echo \"[0] $MSG_SCRIPTS_TASK_START " + command + " \" | tee -a $OGLOGFILE $OGLOGSESSION \n " +
command + " | tee -a $OGLOGCOMMAND \n ";
//cloneRemoteFromMaster " + form.ipMaster.value + " " + form.source.value + "  " + protocol  + " " + form.targetpart.value + " " + form.tool.value + " " + form.compresor.value + " | tee -a $OGLOGCOMMAND \n";
//form.codigo.value="cloneRemoteFromMaster " + form.ipMaster.value + " " + form.source.value + "  " + protocol  + " " + form.targetpart.value + " " + form.tool.value + " " + form.compresor.value;
}

function codeDeployImage(form){
switch (form.idmetodo.value)
{
	case "MULTICAST":
 		protocol="MULTICAST " + form.mcastpuerto.value  + ":" + form.mcastmodo.value + ":" + form.mcastdireccion.value + ":" + form.mcastvelocidad.value + "M:" + form.mcastnclien.value + ":" + form.mcastseg.value + " ";
		break;
	case "TORRENT":
		protocol=" TORRENT " +  form.modp2p.value + ":" + form.timep2p.value;
		break;
	case "UNICAST":
		protocol=" UNICAST";
		break;
}
//form.codigo.value="deployImage REPO /";
if (form.modo[0].checked) 
{
	command="deployImage REPO /" + form.idimagen.value + " 1 " + form.idparticion.value + " " + protocol  ;
	form.codigo.value="\
echo [0] $MSG_SCRIPTS_TASK_START " + command +" | tee -a $OGLOGSESSION \n \ " +
command + " | tee -a $OGLOGCOMMAND";
	//form.codigo.value="deployImage REPO /" + form.idimagen.value + " 1 " + form.idparticion.value + " " + protocol  ;
}
else
{
	command="updateCache REPO /" + form.idimagen.value + ".img" + " " + protocol  ;
	form.codigo.value="\
echo [0] $MSG_SCRIPTS_TASK_START " + command +" | tee -a $OGLOGSESSION \n \ " +
command + " | tee -a $OGLOGCOMMAND";		
	//form.codigo.value="updateCache REPO /" + form.idimagen.value + ".img" + " " + protocol  ;
}

}

function codeParticionado(form){
	var n_disk = form.n_disk.value;
	var tipo_part_table = form.tipo_part_table.value;
	// Comprobamos si la opcion elejida es GPT o MSDOS para llamar a una funcion u otra
	if(tipo_part_table == "GPT"){
		codeParticionadoGPT(form);
	}
	else{
		codeParticionadoMSDOS(form);
	}
}


function codeParticionadoMSDOS (form) {
	var partCode="";
	var logicalCode="";
	var cacheCode;
	var cacheSize;
	var extended=false;
	var n_disk = form.n_disk.value;
	var tipo_part_table = form.tipo_part_table.value;


	for (var nPart=1; nPart<4; nPart++) {
		var partCheck=eval("form.check"+nPart);
		if (partCheck.checked) {
			var partType=eval("form.part"+nPart);
			if (partType.value == "CUSTOM" ) {
				var partTypeCustom=eval("form.part"+nPart+"custom");
				partCode += " " + partTypeCustom.value;
				if (partTypeCustom.value == "EXTENDED") {
					extended=true;
				}
			} else {
				partCode += " " + partType.value;
				if (partType.value == "EXTENDED") {
					extended=true;
				}
			}
			var partSize=eval("form.size"+nPart);
			if (partSize.value == "CUSTOM" ) {
				var partSizeCustom=eval("form.size"+nPart+"custom");
				partCode += ":" + partSizeCustom.value;
			} else {
				partCode += ":" + partSize.value;
			}
		} else {
			partCode += " EMPTY:0";
		}
	}
	if (form.check4.checked) {
		if (form.size4.value == "0") {
			cacheCode="\
echo \"[20] $MSG_HELP_ogGetPartitionSize CACHE\" | tee -a $OGLOGSESSION \n \
sizecache=`ogGetPartitionSize 1 4` \n \
echo \"[30] $MSG_HELP_ogDeletePartitionTable - $MSG_HELP_ogUpdatePartitionTable 1\" | tee -a $OGLOGSESSION \n \
ogDeletePartitionTable "+n_disk+"  \n \
ogUpdatePartitionTable "+n_disk+" | tee -a $OGLOGCOMMAND \n \
echo \"[50] $MSG_HELP_ogCreateCache \" | tee -a $OGLOGSESSION \n \
initCache $sizecache  &>/dev/null  | tee -a $OGLOGCOMMAND \n ";		
		} else {
			if (form.size4.value == "CUSTOM") { 
				cacheSize = form.size4custom.value; 
			} else {
				cacheSize = form.size4.value;
			} 
			cacheCode="\
echo \"[30] $MSG_HELP_ogDeletePartitionTable - $MSG_HELP_ogUpdatePartitionTable 1\" | tee -a $OGLOGSESSION \n \
ogDeletePartitionTable "+n_disk+"  \n \
ogUpdatePartitionTable "+n_disk+" \n \
echo \"[50]  $MSG_HELP_ogCreateCache \" | tee -a $OGLOGSESSION \n \
initCache "  + cacheSize + " &>/dev/null  | tee -a $OGLOGCOMMAND";	
		} 
	} else {
		cacheCode="\
echo \"[30] $MSG_HELP_ogDeletePartitionTable - $MSG_HELP_ogUpdatePartitionTable 1\" | tee -a $OGLOGSESSION \n \
ogDeletePartitionTable "+n_disk+" \n \
ogUpdatePartitionTable "+n_disk+" \n";
partCode += " EMPTY:0";
	}
	if (extended) {
		var lastLogical=5;
		for (var nPart=9; nPart>5; nPart--) {
			if (eval ("form.check"+nPart+".checked")) {
				lastLogical = nPart;
				break;
			}
		}
		for (var nPart=5; nPart<=lastLogical; nPart++) {
			var partCheck=eval("form.check"+nPart);
			if (partCheck.checked) {
				var partType=eval("form.part"+nPart);
				if (partType.value == "CUSTOM" ) {
					var partTypeCustom=eval("form.part"+nPart+"custom");
					logicalCode += " " + partTypeCustom.value;
				} else {
					logicalCode += " " + partType.value;
				}
				var partSize=eval("form.size"+nPart);
				if (partSize.value == "CUSTOM" ) {
					var partSizeCustom=eval("form.size"+nPart+"custom");
					logicalCode += ":" + partSizeCustom.value;
				} else {
					logicalCode += ":" + partSize.value;
				}
			} else {
				logicalCode += " EMPTY:0";
			}
		}
		partCode += logicalCode;
	}

	form.codigo.value="\
ogCreatePartitionTable "+n_disk+" "+tipo_part_table +" \n \
echo \"[0]  $MSG_HELP_ogCreatePartitions \" | tee -a $OGLOGSESSION $OGLOGFILE \n \
echo \"[10] $MSG_HELP_ogUnmountAll "+n_disk+"\" | tee -a $OGLOGSESSION \n \
ogUnmountCache \n \
ogUnmountAll "+n_disk+" \n  \
" + cacheCode + " \n \
echo \"[60] $MSG_HELP_ogListPartitions "+n_disk+"\" | tee -a $OGLOGSESSION \n \
ogListPartitions "+n_disk+" | tee -a $OGLOGCOMMAND $OGLOGSESSION \n \
echo \"[70] $MSG_HELP_ogCreatePartitions  " + partCode + " \" | tee -a $OGLOGSESSION \n \
ogCreatePartitions "+n_disk+" " + partCode + " | tee -a $OGLOGCOMMAND \n \
echo \"[80] $MSG_HELP_ogSetPartitionActive "+n_disk+" 1\" | tee -a $OGLOGSESSION \n \
ogSetPartitionActive "+n_disk+" 1 \n \
echo \"[100] $MSG_HELP_ogListPartitions  "+n_disk+"\" | tee -a $OGLOGSESSION $OGLOGFILE \n \
ogUpdatePartitionTable "+n_disk+" \n \
ms-sys /dev/sda | grep unknow && ms-sys /dev/sda \n \
ogListPartitions "+n_disk+" | tee -a $OGLOGCOMMAND $OGLOGSESSION $OGLOGFILE \
reboot \n";
}


function codeParticionadoGPT (form) {
        var partCode="";
        var logicalCode="";
        var cacheCode;
        var cacheSize;
        var extended=false;
        var n_disk = form.n_disk.value;
        var tipo_part_table = form.tipo_part_table.value;
		numParts=document.getElementById("numGPTpartitions").value;
		
        for (var nPart=1; nPart <= numParts; nPart++) {
                var partCheck=eval("form.checkGPT"+nPart);
                if (partCheck.checked) {
			// Distinguimos entre cache y el resto de particiones
			if(nPart == 4) {
				if (form.sizeGPT4.value == "0") {
					cacheCode="\
echo \"[20] $MSG_HELP_ogGetPartitionSize CACHE\" | tee -a $OGLOGSESSION \n \
sizecache=`ogGetPartitionSize 1 4` \n \
echo \"[30] $MSG_HELP_ogDeletePartitionTable - $MSG_HELP_ogUpdatePartitionTable 1\" | tee -a $OGLOGSESSION \n \
ogDeletePartitionTable "+n_disk+"  \n \
ogUpdatePartitionTable "+n_disk+" | tee -a $OGLOGCOMMAND \n \
echo \"[50] $MSG_HELP_ogCreateCache \" | tee -a $OGLOGSESSION \n \
initCache $sizecache  &>/dev/null  | tee -a $OGLOGCOMMAND \n ";
				} else {
					if (form.sizeGPT4.value == "CUSTOM") {
						cacheSize = form.sizeGPT4custom.value;
					} else {
						cacheSize = form.sizeGPT4.value;
					}
					cacheCode="\
echo \"[30] $MSG_HELP_ogDeletePartitionTable - $MSG_HELP_ogUpdatePartitionTable 1\" | tee -a $OGLOGSESSION \n \
ogDeletePartitionTable "+n_disk+"  \n \
ogUpdatePartitionTable "+n_disk+" \n \
echo \"[50]  $MSG_HELP_ogCreateCache \" | tee -a $OGLOGSESSION \n \
initCache "  + cacheSize + " &>/dev/null  | tee -a $OGLOGCOMMAND";
				}
			} else{
				var partType=eval("form.partGPT"+nPart);
				if (partType.value == "CUSTOM" ) {
					var partTypeCustom=eval("form.partGPT"+nPart+"custom");
					partCode += " " + partTypeCustom.value;
				} else {
					partCode += " " + partType.value;
				}
				var partSize=eval("form.sizeGPT"+nPart);
				if (partSize.value == "CUSTOM" ) {
					var partSizeCustom=eval("form.sizeGPT"+nPart+"custom");
					partCode += ":" + partSizeCustom.value;
				} else {
					partCode += ":" + partSize.value;
				}
			}
                } else {
			if(nPart == 4){
				cacheCode="\
echo \"[30] $MSG_HELP_ogDeletePartitionTable - $MSG_HELP_ogUpdatePartitionTable 1\" | tee -a $OGLOGSESSION \n \
ogDeletePartitionTable "+n_disk+" \n \
ogUpdatePartitionTable "+n_disk+" \n";
partCode += " EMPTY:0";
			} else{
                        	partCode += " EMPTY:0";
			}
                }
        }
	form.codigo.value="\
ogCreatePartitionTable "+n_disk+" "+tipo_part_table +" \n \
echo \"[0]  $MSG_HELP_ogCreatePartitions \" | tee -a $OGLOGSESSION $OGLOGFILE \n \
echo \"[10] $MSG_HELP_ogUnmountAll "+n_disk+"\" | tee -a $OGLOGSESSION \n \
ogUnmountCache \n \
ogUnmountAll "+n_disk+" \n  \
" + cacheCode + " \n \
echo \"[60] $MSG_HELP_ogListPartitions "+n_disk+"\" | tee -a $OGLOGSESSION \n \
ogListPartitions "+n_disk+" | tee -a $OGLOGCOMMAND $OGLOGSESSION \n \
echo \"[70] $MSG_HELP_ogCreatePartitions  " + partCode + " \" | tee -a $OGLOGSESSION \n \
ogCreatePartitions "+n_disk+" " + partCode + " | tee -a $OGLOGCOMMAND \n \
echo \"[80] $MSG_HELP_ogSetPartitionActive "+n_disk+" 1\" | tee -a $OGLOGSESSION \n \
ogSetPartitionActive "+n_disk+" 1 \n \
echo \"[100] $MSG_HELP_ogListPartitions  "+n_disk+"\" | tee -a $OGLOGSESSION $OGLOGFILE \n \
ogUpdatePartitionTable "+n_disk+" \n \
ms-sys /dev/sda | grep unknow && ms-sys /dev/sda \n \
ogListPartitions "+n_disk+" | tee -a $OGLOGCOMMAND $OGLOGSESSION $OGLOGFILE \
reboot \n";
}


function showPartitionForm (tipo_table_part) {
	document.getElementById("form"+tipo_table_part).style.display="inline";
	if(tipo_table_part == "MSDOS"){
		// De los dos tipos, se oculta el otro
		document.getElementById("formGPT").style.display="none";
	} else{
		document.getElementById("formMSDOS").style.display="none";
	}
}


// Código de pulsación de selección de partición.
function clickPartitionCheckbox (form, npart, isGPT) {
	// Si el parametro no esta definido, se toma como false
	isGPT = (isGPT)?isGPT:"false";
	if(isGPT == true){
		prefix="GPT";
	} else {
		prefix="";
	}
	var partCheck=eval("form.check"+prefix+npart);
	var partType=eval("form.part"+prefix+npart);
	var partSize=eval("form.size"+prefix+npart);
	var partTypeCustom=eval("form.part"+prefix+npart+"custom");
	var partSizeCustom=eval("form.size"+prefix+npart+"custom");
	var freeDisk=document.getElementById("freedisk"+prefix);
	//var logical=document.getElementById("logicas"+prefix);
	if (partCheck.checked) {
		partType.disabled=false;
		partSize.disabled=false;
		if(npart != 4){
			if (partType.options[partType.selectedIndex].value == "CUSTOM") {
				partTypeCustom.disabled=false;
			}
		}
		if (partSize.options[partSize.selectedIndex].value == "CUSTOM") {
			partSizeCustom.disabled=false;
		} else {
			partSizeCustom.disabled=true;
		}
	} else {
		partType.disabled=true;
		partSize.disabled=true;
		partTypeCustom.disabled=true;
		partSizeCustom.disabled=true;
	}
	if (npart <= 4) {
		// Si el formulario es GPT no hay extendidas
		if(isGPT != true){
			checkExtendedPartition(form);
		}
		calculateFreeDisk(form);
	}
}


// Código para calcular el espacio libre del disco.
function calculateFreeDisk(form) {
	// Si esta seleccionada la opcion GPT, se llama a la funcion correspondiente
	if(document.getElementById("tipo_part_table").value == "GPT"){
		calculateFreeGPTDisk(form);
	}
	var freeDisk=document.getElementById("freedisk");
	freeDisk.value=form.minsize.value;
	for (npart=1; npart<=4; npart++) {
		var partCheck=eval("form.check"+npart);
		var partSize=eval("form.size"+npart);
		var partSizeCustom=eval("form.size"+npart+"custom");
		if (partCheck.checked) {
			if (partSize.options[partSize.selectedIndex].value == "CUSTOM") {
				freeDisk.value -= parseInt(partSizeCustom.value);
			} else {
				freeDisk.value -= parseInt(partSize.options[partSize.selectedIndex].value);
			}
		}
	}
	if (parseInt(freeDisk.value) < 0) {
		freeDisk.style.fontWeight = "bold";
		freeDisk.style.fontStyle = "italic";
	} else {
		freeDisk.style.fontWeight = "normal";
		freeDisk.style.fontStyle = "normal";
	}
	if (form.size4.value == 0) {
		freeDisk.value += " (- cache)";		// Aviso de caché sin modificar.
	}
}

// Código para calcular el espacio libre del disco. en el formulario GPT
function calculateFreeGPTDisk(form) {
	// Si esta seleccionada la opcion MSDOS, se llama a la funcion correspondiente
        if(document.getElementById("tipo_part_table").value == "MSDOS"){
                calculateFreeDisk(form);
        }

	var freeDisk=document.getElementById("freediskGPT");
        freeDisk.value=form.minsize.value;
	// Capturamos el numero de particiones que hay hechas
	numParts=document.getElementById("numGPTpartitions").value;
        for (npart=1; npart<=numParts; npart++) {
                var partCheck=eval("form.checkGPT"+npart);
                var partSize=eval("form.sizeGPT"+npart);
                var partSizeCustom=eval("form.sizeGPT"+npart+"custom");
                if (partCheck.checked) {
                        if (partSize.options[partSize.selectedIndex].value == "CUSTOM") {
                                freeDisk.value -= parseInt(partSizeCustom.value);
                        } else {
                                freeDisk.value -= parseInt(partSize.options[partSize.selectedIndex].value);
                        }
                }
        }
        if (parseInt(freeDisk.value) < 0) {
                freeDisk.style.fontWeight = "bold";
                freeDisk.style.fontStyle = "italic";
        } else {
                freeDisk.style.fontWeight = "normal";
                freeDisk.style.fontStyle = "normal";
        }
        if (form.size4.value == 0) {
                freeDisk.value += " (- cache)";         // Aviso de caché sin modificar.
        }
}

// Agrega una nueva fila a la tabla de particiones con una nueva particion
function addGPTPartition(){
	var partitionTypes = "";
	partitionTypes+='<OPTION value="WINDOWS"> Windows </OPTION>';
	partitionTypes+='<OPTION value="WIN-RESERV"> Windows Reserved </OPTION>';
	partitionTypes+='<OPTION value="LINUX"> Linux </OTION>';
	partitionTypes+='<OPTION value="LINUX-RESERV"> Linux Reserved </OPTION>';
	partitionTypes+='<OPTION value="LINUX-SWAP"> Linux Swap </OPTION>';
	partitionTypes+='<OPTION value="LINUX-RAID"> Linux RAID </OPTION>';
	partitionTypes+='<OPTION value="LINUX-LVM"> Linux LVM </OPTION>';
	partitionTypes+='<OPTION value="CHROMEOS"> ChromeOS </OTION>';
	partitionTypes+='<OPTION value="CHROMEOS-KRN"> ChromeOS Kernel </OPTION>';
	partitionTypes+='<OPTION value="CHROMEOS-RESERV"> ChromeOS Reserved </OPTION>';
	partitionTypes+='<OPTION value="HFS"> MacOS HFS </OPTION>';
	partitionTypes+='<OPTION value="HFS-RAID"> MacOS HFS RAID </OPTION>';
	partitionTypes+='<OPTION value="FREEBSD"> FreeBSD </OPTION>';
	partitionTypes+='<OPTION value="FREEBSD-DISK"> FreeBSD Disk </OPTION>';
	partitionTypes+='<OPTION value="FREEBSD-BOOT"> FreeBSD Boot </OPTION>';
	partitionTypes+='<OPTION value="FREEBSD-SWAP"> FreeBSD Swap </OPTION>';
	partitionTypes+='<OPTION value="SOLARIS"> Solaris </OPTION>';
	partitionTypes+='<OPTION value="SOLARIS-DISK"> Solaris Disk </OPTION>';
	partitionTypes+='<OPTION value="SOLARIS-BOOT"> Solaris Boot </OPTION>';
	partitionTypes+='<OPTION value="SOLARIS-SWAP"> Solaris Swap </OPTION>';
	partitionTypes+='<OPTION value="EFI"> EFI </OPTION>';
	partitionTypes+='<OPTION value="MBR"> MBR </OPTION>';
	partitionTypes+='<OPTION value="BIOS-BOOT"> BIOS Boot </OPTION>';


	table = document.getElementById("particionesGPT");
	// Capturamos el numero de particiones, antes incrementamos
	document.getElementById("numGPTpartitions").value = parseInt(document.getElementById("numGPTpartitions").value)+1
	numPart=document.getElementById("numGPTpartitions").value;
	partitionRow = table.insertRow(-1);
	partitionRow.id = "trPartition"+numPart;
	partitionRow.innerHTML="<td> \
<input type='checkbox' name='checkGPT"+numPart+"' value='checkGPT"+numPart+"' onclick='clickPartitionCheckbox(this.form, "+numPart+",true);' /> Partici&oacute;n "+numPart+"</td> \
<td>\
<select name='partGPT"+numPart+"' id='partGPT"+numPart+"' style='width:220' disabled='true' onclick=' \
        if (this.options[this.selectedIndex].value == \'CUSTOM\') { \
                this.form.partGPT"+numPart+"custom.disabled=false; \
        } else { \
                this.form.partGPT"+numPart+"custom.disabled=true; \
        }'><option value='CUSTOM'> Personalizar </option> \
</select> \
<br> \
<select name='partGPT"+numPart+"custom' id='partGPT"+numPart+"custom' style='width:220' disabled='true' >"+partitionTypes+"</select> \
</td> \
<td> \
<select name='sizeGPT"+numPart+"' id='sizeGPT"+numPart+"' style='width:220' disabled='true' onclick=' \
        if (this.form.size"+numPart+".options[this.form.size"+numPart+".selectedIndex].value == \'CUSTOM\') { \
                this.form.sizeGPT"+numPart+"custom.disabled=false; \
        } else { \
                this.form.sizeGPT"+numPart+"custom.disabled=true; \
        } \
' onchange='calculateFreeGPTDisk(this.form);'>0<option value='CUSTOM'> Personalizar </option> \
</select> \
<br /> \
<input type='text' style='width:100' name='sizeGPT"+numPart+"custom' value='0' disabled='true' onchange='calculateFreeDisk(this.form);' /> \
</td>"

}

// Agrega una nueva fila a la tabla de particiones con una nueva particion
function deleteGPTPartition(){
	table = document.getElementById("particionesGPT");
        // Capturamos el numero de particiones
	numPart=document.getElementById("numGPTpartitions").value;
	// Si ya solo quedan 4 particiones, no se elimina ni se decrementa el contador
	if(numPart > 4){
        	partitionRow = document.getElementById("trPartition"+numPart);
		table.deleteRow(partitionRow.rowIndex);
		// Decrementamos el numero de particiones
		document.getElementById("numGPTpartitions").value = parseInt(document.getElementById("numGPTpartitions").value)-1;
	}
}

// Código para comprobar si hay partición extendida activa para mostrar las lógicas.
function checkExtendedPartition(form) {
	var logical=document.getElementById("logicas");
	var visible=false;
	for (npart=1; npart<4; npart++) {
		var partCheck=eval("form.check"+npart);
		var partType=eval("form.part"+npart);
		var partTypeCustom=eval("form.part"+npart+"custom");
		if (partCheck.checked) {
			partType.style.fontWeight = "normal";
			partTypeCustom.style.fontWeight = "normal";
			if (partType.value == "EXTENDED") {
				visible=true;
				partType.style.fontWeight = "bold";
			}
			if (partType.value == "CUSTOM" && partTypeCustom.value == "EXTENDED") {
				visible=true;
				partTypeCustom.style.fontWeight = "bold";
			}
		}
	}
	if (visible) {
		logical.style.visibility="visible";
	} else {
		logical.style.visibility="hidden";
	}
}

