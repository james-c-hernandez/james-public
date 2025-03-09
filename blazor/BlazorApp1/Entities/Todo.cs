using System.ComponentModel.DataAnnotations;

namespace BlazorApp1.Entities
{
    public class Todo
    {
        [Key]
        public int Id { get; set; }
        public string Title { get; set; }
        public bool IsDone { get; set; }
    }
}
