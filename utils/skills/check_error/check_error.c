/*	check_error.c
 *
 *	Pequeña utilidad para verificar un código de estado proporcionado por
 *	un servidor externo.
 *
 */

char * check_error(int errcod) {
	char * msg;

	switch (errcod) {
		case 1:
			msg = "It seems that there is an error on the server or the current operation is not allowed";
			break;

		case 2:
			msg = "There was an error opening the remote directory";
			break;

		case 3:
			msg = "Error opening remote file";
			break;

		case 4:
			msg = "The file already exists!";
			break;

		case 5:
			msg = "Error in the actions!";
			break;

		case 0:
			msg = "Successful operation!";
			break;

		default:
			msg = "Unknown error";
			break;
	
	}

	return msg;

}
