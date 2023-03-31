package IoT_Project;

import java.awt.*;
import javax.swing.*;

class SCMWindow extends JFrame {
	
	private static final long serialVersionUID = 1L;
	private JButton refill = new JButton("Refill");
	private JButton recover = new JButton("Recover");
	private JTextArea Mstate = new JTextArea();
	
	public SCMWindow() {
		super("Smart Coffee Machine");
		setSize(400, 100);
		this.setMinimumSize(new Dimension(400, 100));
		
		JPanel mainPanel = new JPanel();
		JPanel buttonPanel = new JPanel();
		JPanel textPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.PAGE_AXIS));
		this.getContentPane().add(buttonPanel);
		this.getContentPane().add(buttonPanel);
		this.getContentPane().add(mainPanel);
		
		Mstate.setEditable(false);
		refill.setEnabled(false);
		recover.setEnabled(false);
		
		Mstate.setRows(1);
		Mstate.setColumns(20);
		
		buttonPanel.add(refill);
		buttonPanel.add(recover);
		textPanel.add(Mstate);
		mainPanel.add(buttonPanel);
		mainPanel.add(textPanel);
		
		setVisible(true);
	}
	
	public void UpdateState(String temp) {
		Mstate.setText(temp);
	}
	
	public void setRefillEnable() {
		refill.setEnabled(true);
	}
	
	public void setRecoverEnable() {
		recover.setEnabled(true);
	}
	
	public void setRefillDisable() {
		refill.setEnabled(false);
	}
	
	public void setRecoverDisable() {
		recover.setEnabled(false);
	}
	
	public JButton getRefill() {
		return refill;
	}
	
	public JButton getRecover() {
		return recover;
	}
}