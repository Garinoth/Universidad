// Interfaz del servicio DFS

package dfs;
import java.io.FileNotFoundException;
import java.rmi.*;

public interface DFSServicio extends Remote {
    DFSFicheroServ abrir ( String nombre, String modo ) throws RemoteException, FileNotFoundException;
}       
