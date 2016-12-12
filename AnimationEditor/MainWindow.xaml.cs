using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;

//[DllImport(@"ChromaAnimationLibrary.dll", CharSet = CharSet.Unicode)]


namespace AnimationEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    //TODO: Put buttons in a 2D List in accordance to Razer Chroma arrays.

    

    public partial class MainWindow : Window
    {
        const int KEYBOARD_ROWS = 6;
        const int KEYBOARD_COLUMNS = 22;

        List<ColorPickerWindow> ActiveColorPickers = new List<ColorPickerWindow>();
        List<List<Button>> KeyboardLayout = new List<List<Button>>();

        public MainWindow()
        {
            InitializeComponent();
            for (int i = 0; i < KEYBOARD_ROWS; ++i)
                KeyboardLayout.Add(new List<Button>());
        }

        private void OpenColorPicker(object sender, RoutedEventArgs e)
        {
            ColorPickerWindow Color_picker = new ColorPickerWindow();
            Color_picker.SelectedKey = sender;
            Color_picker.Show();

            foreach(ColorPickerWindow picker in ActiveColorPickers)
                picker.Activate();

            Color_picker.Activate();
        }

        private void OnClose(object sender, System.ComponentModel.CancelEventArgs e)
        {
            App.Current.Shutdown();
        }

        public void AddNewColorPicker(ColorPickerWindow picker)
        {
            ActiveColorPickers.Add(picker);
        }

        public void RemoveColorPicker(ColorPickerWindow picker)
        {
            ActiveColorPickers.Remove(picker);
        }

        private void OnKeyboardChange(object sender, SelectionChangedEventArgs e)
        {
            ComboBoxItem Keyboard = e.AddedItems[0] as ComboBoxItem;
            switch(Keyboard.Name)
            {
                case "Ornata":
                    ToggleMacroKeys(false);
                    ToggleNumPadKeys(true);
                    break;
                case "Deathstalker":
                    ToggleMacroKeys(false);
                    ToggleNumPadKeys(true);
                    break;
                case "Blackwidow":
                    ToggleMacroKeys(true);
                    ToggleNumPadKeys(true);
                    break;
                case "Blackwidow_X":
                    ToggleMacroKeys(false);
                    ToggleNumPadKeys(true);
                    break;
                case "Blackwidow_Tournament":
                    ToggleMacroKeys(false);
                    ToggleNumPadKeys(false);
                    break;
                case "Blackwidow_X_Tournament":
                    ToggleMacroKeys(false);
                    ToggleNumPadKeys(false);
                    break;
                case "Blackwidow_DeusEx":
                    ToggleMacroKeys(false);
                    ToggleNumPadKeys(true);
                    break;
                case "Blackwidow_Overwatch":
                    ToggleMacroKeys(false);
                    ToggleNumPadKeys(true);
                    break;
                case "Blackwidow_CoD":
                    ToggleMacroKeys(true);
                    ToggleNumPadKeys(true);
                    break;
                default:
                    break;
            }
        }

        private void ToggleMacroKeys(bool pred)
        {
            M1.IsEnabled = pred;
            M2.IsEnabled = pred;
            M3.IsEnabled = pred;
            M4.IsEnabled = pred;
            M5.IsEnabled = pred;
        }

        private void ToggleNumPadKeys(bool pred)
        {
            NumLock. IsEnabled = pred;
            NumDivide.IsEnabled = pred;
            NumMultiply.IsEnabled = pred;
            NumMinus.IsEnabled = pred;
            NumPlus.IsEnabled = pred;
            NumEnter.IsEnabled = pred;
            Num7.IsEnabled = pred;
            Num8.IsEnabled = pred;
            Num9.IsEnabled = pred;
            Num4.IsEnabled = pred;
            Num5.IsEnabled = pred;
            Num6.IsEnabled = pred;
            Num0.IsEnabled = pred;
            Num1.IsEnabled = pred;
            Num2.IsEnabled = pred;
            Num3.IsEnabled = pred;
            NumPeriod.IsEnabled = pred;
        }

        private void LoadKeyboardInfo(object sender, EventArgs e)
        {
            //Add buttons to KeyboardLayout in correct order
            //Row 0

            //Row 1
            //Row 2
            //Row 3
            //Row 4
            //Row 5

        }
    }
}
