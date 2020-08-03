using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Reflection;
using System.Resources;
using System.Runtime.CompilerServices;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

using GameStore;

namespace GameStore.Model
{
   
    [ContentProperty("Text")]
    public class Translator : INotifyPropertyChanged
    {
        public string this[string text]
        {
            get
            {

                return Resources.ResourceManager.GetString(text, Resources.Culture);
            }
        }

        public static Translator Instance { get; } = new Translator();

        public event PropertyChangedEventHandler PropertyChanged;

        public void Invalidate()
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(null));
        }
    }


}
