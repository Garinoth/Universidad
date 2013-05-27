// Clase de servidor que implementa el API de acceso remoto a un fichero

package dfs;
import java.rmi.*;
import java.rmi.server.*;

import java.io.*;

@SuppressWarnings("serial")
public class DFSFicheroServImpl extends UnicastRemoteObject implements DFSFicheroServ {
    private static final String DFSDir = "DFSDir/";
    
    private RandomAccessFile f;
    
    public DFSFicheroServImpl(String nombre, String modo) throws RemoteException, FileNotFoundException {
    	this.f = new RandomAccessFile(DFSDir + nombre, modo);
    }

	@Override
	public Object[] read( int tam ) throws RemoteException, IOException {
		byte[] b = new byte [tam];
		int leido = f.read(b);
		Object[] res = new Object[2];
		res[0] = leido;
		res[1] = b;
		return res;
	}

	@Override
	public void write(byte[] b) throws RemoteException, IOException {
		f.write(b);		
	}

	@Override
	public void seek(long p) throws RemoteException, IOException {
		f.seek(p);
	}

	@Override
	public void close() throws RemoteException, IOException {
		f.close();
	}
}
