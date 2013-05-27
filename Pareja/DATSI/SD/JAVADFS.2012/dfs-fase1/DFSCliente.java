// Clase de cliente que proporciona acceso al servicio DFS

package dfs;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

public class DFSCliente {	
	private int tamBloque;
	private int tamCache;
	
	private DFSServicio servicio;
	
    public DFSCliente(int tamBloque, int tamCache) {
    	this.tamBloque = tamBloque;
    	this.tamCache = tamCache;
    	try {
    		String reg = this.getRegistry();
			this.servicio = (DFSServicio) Naming.lookup(reg);
		} catch (MalformedURLException e) {
			e.printStackTrace();
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (NotBoundException e) {
			e.printStackTrace();
		}
    }
    
    public String getRegistry() {
    	String servidor = System.getenv("SERVIDOR");
    	String puerto = System.getenv("PUERTO");
    	final String reg = "//" + servidor + ":" + puerto + "/DFS";
		return reg;
    }
    
	public DFSServicio getServicio() {
		return this.servicio;
	}

	public int getTamBloque() {
		return tamBloque;
	}

	public int getTamCache() {
		return tamCache;
	}
}

