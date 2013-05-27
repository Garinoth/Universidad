// Clase de cliente que proporciona el API del servicio DFS

package dfs;

import java.io.*;
import java.rmi.*;

public class DFSFicheroCliente  {
	
	private DFSCliente dfs;
	private DFSServicio srv;
	private DFSFicheroServ f;
	
    public DFSFicheroCliente(DFSCliente dfs, String nombre, String modo) throws RemoteException, IOException, FileNotFoundException {
    	this.dfs = dfs;
    	this.srv = dfs.getServicio();
    	this.f = (DFSFicheroServ) srv.abrir(nombre, modo);
    }
    public int read(byte[] b) throws RemoteException, IOException {
    	Object[] buff = new Object[2];
    	buff = f.read(b.length);
    	Integer leido = (Integer) buff[0];
    	if (leido > 0) System.arraycopy(buff[1], 0, b, 0, leido);
    	else leido = 0;
    	return (int)leido;
    }
    public void write(byte[] b) throws RemoteException, IOException {
    	f.write(b);
    }
    public void seek(long p) throws RemoteException, IOException {
    	f.seek(p);
    }
    public void close() throws RemoteException, IOException {
    	f.close();
    }
}
