// Interfaz del API de acceso remoto a un fichero

package dfs;
import java.io.IOException;
import java.rmi.*;
import java.util.ArrayList;

public interface DFSFicheroServ extends Remote  {
	Object[] read( int tam ) throws RemoteException, IOException;
	void write( byte[] b ) throws RemoteException, IOException;
	void seek( long p ) throws RemoteException, IOException;
	void close() throws RemoteException, IOException;
	long getLastModifiedDate() throws RemoteException;
}
