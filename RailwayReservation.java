import javax.swing.*;
import java.awt.*;
import java.util.*;

record Passenger(String name, int age, String gender, int seatNumber, int pnr) {
    public String toString() { return name + " (Age:" + age + ", " + gender + ", PNR:" + pnr + ")"; }
}

class TrainClass {
    String className;
    boolean[] seats;
    ArrayList<Passenger> passengers = new ArrayList<>();
    int nextPNR = 1001;

    TrainClass(String className, int seatCount) {
        this.className = className;
        seats = new boolean[seatCount];
    }

    String bookSeat(int idx, String name, int age, String gender) {
        if (idx < 0 || idx >= seats.length) return "Invalid seat number!";
        if (seats[idx]) return "Seat already booked!";
        passengers.add(new Passenger(name, age, gender, idx + 1, nextPNR));
        seats[idx] = true;
        return "✅ Seat " + (idx + 1) + " booked successfully!\nPNR: " + nextPNR++;
    }

    String cancelTicket(int pnr) {
        return passengers.stream()
            .filter(p -> p.pnr() == pnr)
            .findFirst()
            .map(p -> {
                seats[p.seatNumber() - 1] = false;
                passengers.remove(p);
                return "❌ Ticket with PNR " + pnr + " cancelled.";
            })
            .orElse("⚠️ PNR not found!");
    }

    Passenger findBySeat(int seatNo) {
        return passengers.stream().filter(p -> p.seatNumber() == seatNo).findFirst().orElse(null);
    }
}

class Train {
    String name;
    Map<String, TrainClass> classes = new LinkedHashMap<>();

    Train(String name) { this.name = name; }
}

public class RailwayReservationOptimized {
    private final Map<String, Train> trains = new LinkedHashMap<>();
    private JComboBox<String> trainSelect, classSelect;

    public RailwayReservationOptimized() {
        prepareData();
        buildUI();
    }

    private void prepareData() {
        addTrain("Rajdhani Express", "CC (Chair Car)", 30, "EC (Executive Chair)", 30);
        addTrain("Shatabdi Express", "CC (Chair Car)", 30, "EC (Executive Chair)", 30);
        addTrain("Duronto Express", "Sleeper", 30, "AC-3 Tier", 30, "AC-2 Tier", 30, "AC-1 Tier", 30);
    }

    private void addTrain(String name, Object... classesData) {
        Train t = new Train(name);
        for (int i = 0; i < classesData.length; i += 2) {
            t.classes.put((String) classesData[i], new TrainClass((String) classesData[i], (int) classesData[i + 1]));
        }
        trains.put(name, t);
    }

    private void buildUI() {
        JFrame f = new JFrame("Railway Reservation System");
        f.setSize(420, 220);
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.setLayout(new GridBagLayout());
        GridBagConstraints g = new GridBagConstraints();
        g.insets = new Insets(8, 8, 8, 8);
        g.fill = GridBagConstraints.HORIZONTAL;

        trainSelect = new JComboBox<>(trains.keySet().toArray(String[]::new));
        classSelect = new JComboBox<>();
        
        addRow(f, g, 0, "Select Train:", trainSelect);
        addRow(f, g, 1, "Select Class:", classSelect);

        trainSelect.addActionListener(e -> updateClasses());
        updateClasses();

        g.gridx = 0; g.gridy = 2; g.gridwidth = 2;
        JButton viewBtn = new JButton("View Seats");
        f.add(viewBtn, g);
        viewBtn.addActionListener(e -> openSeatMap());

        g.gridy = 3;
        JLabel hint = new JLabel("Click an available seat to book. Click a booked seat to view/cancel.");
        hint.setFont(hint.getFont().deriveFont(Font.PLAIN, 12f));
        f.add(hint, g);

        f.setLocationRelativeTo(null);
        f.setVisible(true);
    }

    private void addRow(JFrame f, GridBagConstraints g, int row, String label, JComponent comp) {
        g.gridx = 0; g.gridy = row; f.add(new JLabel(label), g);
        g.gridx = 1; f.add(comp, g);
    }

    private void updateClasses() {
        classSelect.removeAllItems();
        Train t = trains.get((String) trainSelect.getSelectedItem());
        if (t != null) t.classes.keySet().forEach(classSelect::addItem);
    }

    private void openSeatMap() {
        String trainName = (String) trainSelect.getSelectedItem();
        String className = (String) classSelect.getSelectedItem();
        if (trainName == null || className == null) return;

        TrainClass tc = trains.get(trainName).classes.get(className);
        JFrame sf = new JFrame(trainName + " - " + className);
        sf.setSize(760, 520);
        sf.setLayout(new BorderLayout(8, 8));

        JLabel title = new JLabel(trainName + " — " + className, SwingConstants.CENTER);
        title.setFont(title.getFont().deriveFont(Font.BOLD, 16f));
        sf.add(title, BorderLayout.NORTH);

        JPanel sp = new JPanel(new GridLayout(5, 6, 6, 6));
        sp.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
        refreshSeats(sp, tc, sf, trainName, className);
        sf.add(new JScrollPane(sp), BorderLayout.CENTER);

        JButton showBooked = new JButton("Show Booked Passengers");
        showBooked.addActionListener(e -> showBooked(tc, sf, trainName, className));
        JPanel bottom = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 10));
        bottom.add(showBooked);
        sf.add(bottom, BorderLayout.SOUTH);

        sf.setLocationRelativeTo(null);
        sf.setVisible(true);
    }

    private void refreshSeats(JPanel panel, TrainClass tc, JFrame parent, String train, String cls) {
        panel.removeAll();
        String[] berths = train.equals("Duronto Express") ? getBerths() : null;

        for (int i = 0; i < tc.seats.length; i++) {
            String label = berths != null ? (i + 1) + "-" + berths[i] : String.valueOf(i + 1);
            JButton btn = new JButton("<html><center>" + label + "</center></html>");
            btn.setPreferredSize(new Dimension(80, 50));
            btn.setOpaque(true);
            btn.setBackground(tc.seats[i] ? Color.RED : new Color(144, 238, 144));

            int idx = i;
            btn.addActionListener(e -> handleSeatClick(idx, tc, parent, train, cls, panel, berths));
            panel.add(btn);
        }
        panel.revalidate();
        panel.repaint();
    }

    private String[] getBerths() {
        String[] b = new String[30];
        for (int i = 0; i < 24; i++) b[i] = new String[]{"L", "M", "U"}[i % 3];
        for (int i = 24; i < 30; i++) b[i] = i % 2 == 0 ? "SL" : "SU";
        return b;
    }

    private void handleSeatClick(int idx, TrainClass tc, JFrame parent, String train, String cls, JPanel panel, String[] berths) {
        if (!tc.seats[idx]) {
            JTextField name = new JTextField(), age = new JTextField();
            JComboBox<String> gender = new JComboBox<>(new String[]{"M", "F", "Other"});
            Object[] form = {"Name:", name, "Age:", age, "Gender:", gender};
            
            if (JOptionPane.showConfirmDialog(parent, form, "Book Seat " + (idx + 1) + (berths != null ? "-" + berths[idx] : ""), JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION) {
                try {
                    String nm = name.getText().trim();
                    int ag = Integer.parseInt(age.getText().trim());
                    if (nm.isEmpty() || ag <= 0) throw new Exception();
                    JOptionPane.showMessageDialog(parent, tc.bookSeat(idx, nm, ag, (String) gender.getSelectedItem()));
                    refreshSeats(panel, tc, parent, train, cls);
                } catch (Exception ex) {
                    JOptionPane.showMessageDialog(parent, "Invalid input. Booking canceled.");
                }
            }
        } else {
            Passenger p = tc.findBySeat(idx + 1);
            if (p != null) {
                String info = "Seat " + p.seatNumber() + (berths != null ? "-" + berths[idx] : "") + " is BOOKED.\n" +
                    "Name: " + p.name() + "\nAge: " + p.age() + "\nGender: " + p.gender() + "\nPNR: " + p.pnr();
                
                if (JOptionPane.showOptionDialog(parent, info, "Seat Info", JOptionPane.YES_NO_OPTION, 
                    JOptionPane.INFORMATION_MESSAGE, null, new Object[]{"Cancel Ticket", "Close"}, "Close") == JOptionPane.YES_OPTION) {
                    if (JOptionPane.showConfirmDialog(parent, "Cancel ticket with PNR " + p.pnr() + "?", "Confirm Cancel", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
                        JOptionPane.showMessageDialog(parent, tc.cancelTicket(p.pnr()));
                        refreshSeats(panel, tc, parent, train, cls);
                    }
                }
            }
        }
    }

    private void showBooked(TrainClass tc, JFrame parent, String train, String cls) {
        StringBuilder sb = new StringBuilder();
        tc.passengers.forEach(p -> sb.append("Seat ").append(p.seatNumber()).append(" : ").append(p).append("\n"));
        if (sb.length() == 0) sb.append("No booked passengers in ").append(cls).append(".");
        
        JTextArea ta = new JTextArea(sb.toString());
        ta.setEditable(false);
        JScrollPane sp = new JScrollPane(ta);
        sp.setPreferredSize(new Dimension(480, 320));
        JOptionPane.showMessageDialog(parent, sp, "Booked - " + train + " / " + cls, JOptionPane.INFORMATION_MESSAGE);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(RailwayReservationOptimized::new);
    }
}