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
using System.Windows.Shapes;

namespace AnimationEditor
{
    /// <summary>
    /// Interaction logic for ColorPickerWindow.xaml
    /// </summary>
    public partial class ColorPickerWindow : Window
    {
        public object SelectedKey;
        private bool Registered = false;

        public ColorPickerWindow()
        {
            InitializeComponent();
        }

        private void ColorChanged(object sender, RoutedPropertyChangedEventArgs<Color?> e)
        {
            Button Key = SelectedKey as Button;
            Key.Background = new SolidColorBrush(Canvas.SelectedColor.GetValueOrDefault());
        }

        private void OnActivate(object sender, EventArgs e)
        {
            Button Key = SelectedKey as Button;
            string NewName = Key.Name + " Key Color";
            this.Title = NewName;

            if(!Registered)
            {
                (Application.Current.MainWindow as MainWindow).AddNewColorPicker(this);
                Registered = true;
            }
        }

        private void OnClose(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if(Application.Current.MainWindow != null)
                (Application.Current.MainWindow as MainWindow).RemoveColorPicker(this);
        }
    }
}
