// Clase de cliente que proporciona acceso al servicio DFS

package dfs;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.HashMap;

public class DFSCliente {	
	private int tamBloque;
	private int tamCache;
	private HashMap<String, Cache> caches;
	
	private DFSServicio servicio;
	
    public DFSCliente(int tamBloque, int tamCache) {
    	this.tamBloque = tamBloque;
    	this.tamCache = tamCache;
    	this.caches = new HashMap<String, Cache>();
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
	
	public Cache getCache(String nombre) {
		Cache res;
		if (this.caches.containsKey(nombre)) {
			res = this.caches.get(nombre);
		}
		else {
			res = new Cache(this.tamCache);
			this.caches.put(nombre, res);
		}
		return res;
	}
}

