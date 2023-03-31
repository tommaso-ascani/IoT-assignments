package IoT_Project;

import javax.swing.JButton;
import javax.swing.SwingUtilities;

class SmartCoffeeMachine {
	
	static SCMWindow view = null;
	static String msg = null;
	static String port;
	
	public static void main(String[] args) throws Exception {
		
		port = "COM3";		
		CommChannel channel = new SerialCommChannel(port,9600);		
		
		SwingUtilities.invokeAndWait(() -> {
			view = new SCMWindow();
		});
		
		
		JButton refill = new JButton();	
		JButton recover = new JButton();		
		refill = view.getRefill();
		recover = view.getRecover();
		
		refill.addActionListener(e -> {
        	System.out.println("REFILL");
        	channel.sendMsg("REFILL");
        	view.setRefillDisable();
        	view.UpdateState("Refill Done!");
        });
		
		recover.addActionListener(e -> {
        	System.out.println("RECOVER");
        	channel.sendMsg("RECOVER");
        	view.setRecoverDisable();
        	view.UpdateState("Recover Done!");
        });
		
		
		while (true) {
			msg = channel.receiveMsg();
			System.out.println(msg);
			view.UpdateState(msg);
			
			if(msg.equals("MAINTENANCE: Refill Needed")) {
				view.setRefillEnable();				
			}
			
			if(msg.equals("MAINTENANCE: Recover Needed")) {
				view.setRecoverEnable();				
			}			
		}
	}
}