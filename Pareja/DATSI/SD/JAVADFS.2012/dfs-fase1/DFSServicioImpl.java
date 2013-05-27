// Clase de servidor que implementa el servicio DFS

package dfs;

import java.io.FileNotFoundException;
import java.rmi.*;
import java.rmi.server.*;

@SuppressWarnings("serial")
public class DFSServicioImpl extends UnicastRemoteObject implements DFSServicio {
    public DFSServicioImpl() throws RemoteException {
    }
    public DFSFicheroServ abrir ( String nombre, String modo ) throws RemoteException {
		try {
			return new DFSFicheroServImpl(nombre, modo);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		return null;
    }
}
