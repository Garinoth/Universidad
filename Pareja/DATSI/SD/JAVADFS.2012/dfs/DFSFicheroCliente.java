// Clase de cliente que proporciona el API del servicio DFS

package dfs;

import java.io.*;
import java.rmi.*;

public class DFSFicheroCliente  {

	private DFSServicio srv;
	private DFSFicheroServ f;
	private Cache cache;
	private int tamBloque;
	private long position;
	private String modo;

	public DFSFicheroCliente(DFSCliente dfs, String nombre, String modo) throws RemoteException, IOException, FileNotFoundException {
		this.srv = dfs.getServicio();
		this.modo = modo;
		this.f = (DFSFicheroServ) srv.abrir(nombre, modo);
		this.cache = dfs.getCache(nombre);
		if ( (this.cache != null) && (this.f.getLastModifiedDate() > this.cache.obtenerFecha()) ){ this.cache.vaciar(); System.out.println("CACHE VACIADA");}
		this.tamBloque = dfs.getTamBloque();
		this.position = 0;
	}
	public int read(byte[] b) throws RemoteException, IOException {
		Integer totalLeido = 0;	
		int bloques = b.length / this.tamBloque;

		for (int i = 0; i < bloques; i++) {
			Bloque bloque;
			byte [] bloqueLeido = null;
			Integer leido = 0;
			if ((bloque = this.cache.getBloque(this.position)) != null) {
				leido = this.tamBloque;
			}
			else {
				Object[] buff = new Object[2];
				f.seek(this.position);
				buff = f.read(this.tamBloque);
				leido = (Integer)buff[0];
				if (leido > 0) {
					bloque = new Bloque(this.position, (byte[]) buff[1]);
					Bloque bloqueExtraido = this.cache.putBloque(bloque);
					this.cache.desactivarMod(bloque);
					if (bloqueExtraido != null) actualizarBloque(bloqueExtraido);
				}
			}

			if (leido > 0) {
				bloqueLeido = bloque.obtenerContenido();
				System.arraycopy(bloqueLeido, 0, b, totalLeido, leido);
			}
			else leido = 0;
			this.position += leido;
			totalLeido += leido;
		}
		return (int)totalLeido;
	}

	public void write(byte[] b) throws RemoteException, IOException {
		if (!this.modo.equals("rw")) throw new IOException();		
		int bloques = b.length / this.tamBloque;
		int posBuff = 0;

		for (int i = 0; i < bloques; i++) {
			Bloque bloque;
			byte[] temp = new byte[this.tamBloque];
			System.arraycopy(b, posBuff, temp, 0, this.tamBloque);
			posBuff += this.tamBloque;
			bloque = new Bloque(this.position, temp );
			Bloque bloqueExtraido = this.cache.putBloque(bloque);
			this.cache.activarMod(bloque);
			if (bloqueExtraido != null) actualizarBloque(bloqueExtraido);
			this.position += this.tamBloque;
		}
	}

	public void actualizarBloque( Bloque bloque ) {
		try {
			if (this.cache.preguntarYDesactivarMod(bloque)) { 
				f.seek(bloque.obtenerId());
				f.write(bloque.obtenerContenido());
			}
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void seek(long p) throws RemoteException, IOException {
		this.position = p;
	}

	public void close() throws RemoteException, IOException {
		for (Bloque bloque : this.cache.listaMod()) {
			actualizarBloque(bloque);
		}
		this.cache.vaciarListaMod();
		this.cache.fijarFecha(this.f.getLastModifiedDate());
		f.close();
	}
}
